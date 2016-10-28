/*
 ==============================================================================

 ProgressNotifier.cpp
 Created: 6 Oct 2016 6:30:31pm
 Author:  Martin Hermant

 ==============================================================================
 */

#include "ProgressNotifier.h"




ProgressTask::ProgressTask(String _taskName,ProgressTask * _parentTask ):parentTask(_parentTask),taskName(_taskName),progress(0){}
ProgressTask::~ProgressTask(){masterReference.clear();}

int ProgressTask::getTaskPositionInParent(){
  if(parentTask){
    return parentTask->subTasks.indexOf(this);
  }
  return 0;
}



StringArray ProgressTask::getAddress(){
  StringArray res;
  ProgressTask * pt = this;
  while(pt!=nullptr){
    res.add(pt->taskName);
    pt = pt->parentTask;
  }
  int iter = 0;
  int half =res.size()/2;

  while(iter<half){
    res.getReference(iter).swapWith(res.getReference(res.size()-1-iter));
    iter++;
  }
  return res;
}
void ProgressTask::start(){getRootTask()->taskListeners.call(&TaskListener::taskStarted,this);}
void ProgressTask::end(){getRootTask()->taskListeners.call(&TaskListener::taskEnded,this);}
void ProgressTask::setProgress(float _progress){progress = _progress;getRootTask()->taskListeners.call(&TaskListener::taskProgress,this,progress);}


float ProgressTask::getNormalizedProgress(){
  int totalNum = getRootTask()->getTotalNumberOfTasks();
  if(totalNum == 0 )return 1;
  int localPos = getTaskPosition();
  return (localPos+progress)*1.0f/totalNum;
}
void ProgressTask::clearTasks(){
  subTasks.clear();
}
ProgressTask * ProgressTask::addTask(String _taskName){
  ProgressTask * newT = new ProgressTask(_taskName,this);
  subTasks.add(newT);
  getRootTask()->taskListeners.call(&TaskListener::taskAdded,this);
  return newT;
}


int ProgressTask::getTaskPosition(){
  ProgressTask * checkedT = this;
  if(!isLeaf()){
    // we can "count" only leaves before so a parent woud have the position of its leftmost leaf
    checkedT = getLeftMostLeaf();
  }
  ProgressTask * t = getRootTask()->getLeftMostLeaf();
  int numLeafBeforeMe = 0;
  while(t!=checkedT ){
    t= t->getNextSibling();
    numLeafBeforeMe++;
    if( checkedT==nullptr){
      jassertfalse;
      return 0;
    }
  }
  return numLeafBeforeMe;
}

bool ProgressTask::isASubTaskOf(ProgressTask * pt){
  ProgressTask * checkedTask = this;
  while(checkedTask->parentTask!=nullptr){
    if(pt==checkedTask)return true;
  }
  return false;
}
ProgressTask * ProgressTask::getRootTask(){
  ProgressTask * checkedTask = this;
  while(checkedTask->parentTask!=nullptr){
    checkedTask = checkedTask->parentTask;
  }
  return checkedTask;
}

ProgressTask * ProgressTask::getNextSibling(){
  if(parentTask){
    jassert(parentTask->subTasks.size()>0);
    if(parentTask->subTasks[parentTask->subTasks.size()-1] == this){
      return  parentTask->getNextSibling();
    }
    int curPos = getTaskPositionInParent();
    return parentTask->subTasks[curPos+1]->getLeftMostLeaf();
  }
  return nullptr;
};
ProgressTask * ProgressTask::getLeftMostLeaf(){
  ProgressTask * checkedTask  = this;
  while(!checkedTask->isLeaf()){checkedTask = checkedTask->subTasks[0];};
  return checkedTask;
}


int ProgressTask::getTotalNumberOfTasks(){
  int res = isLeaf()?1:0;
  for(auto & t:subTasks){
    if(isLeaf())res+=1;
    else res+=t->getTotalNumberOfTasks();
  }
  return res;
}

bool ProgressTask::isLeaf(){
  return subTasks.size() ==0;
}


/////////////////////
/// progress Notifier



class StartCallbackMessage : public CallbackMessage{
public:
  StartCallbackMessage(ListenerList<ProgressNotifier::ProgressListener> * _n,ProgressTask * t):n(_n),task(t){}
  void messageCallback() override{if(task.get())n->call(&ProgressNotifier::ProgressNotifier::ProgressListener::startedProgress,task);}
  ListenerList<ProgressNotifier::ProgressListener>* n;
  WeakReference<ProgressTask> task;
};
class EndCallbackMessage : public CallbackMessage{
public:
  EndCallbackMessage(ListenerList<ProgressNotifier::ProgressListener> * _n,ProgressTask * t):n(_n),task(t){}
  void messageCallback() override{if(task.get())n->call(&ProgressNotifier::ProgressListener::endedProgress,task);}
  ListenerList<ProgressNotifier::ProgressListener>* n;
  WeakReference<ProgressTask> task;
};

class ProgressCallbackMessage : public CallbackMessage{
public:
  ProgressCallbackMessage(ListenerList<ProgressNotifier::ProgressListener> * _n,ProgressTask * t,float p):n(_n),progress(p),task(t){}
  void messageCallback() override{if(task.get())n->call(&ProgressNotifier::ProgressListener::newProgress,task,progress);}
  ListenerList<ProgressNotifier::ProgressListener>* n;
  WeakReference<ProgressTask> task;
  float progress;
};



class FakeProgress : Timer{
  public :
  FakeProgress(ProgressTask * _task,float timeToComplete):task(_task){
    const int period = 100;
    step = period*1.0f/timeToComplete;
    curProgress = 0;
    startTimer(period);
  }
  void timerCallback(){
    curProgress+=step;
    task->setProgress( jmin(1.0f,curProgress));
  }
  float step;
  float curProgress;
  ProgressTask * task;
};


ProgressNotifier::ProgressNotifier():currentTask(nullptr),ProgressTask("mainTask"){
  addTaskListener(this);
};
 ProgressNotifier::~ProgressNotifier(){

}


void ProgressNotifier::taskAdded(ProgressTask * ){};
void ProgressNotifier::taskStarted(ProgressTask * t){
  if(currentTask!=t && currentTask!=nullptr){(new EndCallbackMessage(&progressListeners,currentTask))->post();}
  currentTask = t;
  (new StartCallbackMessage(&progressListeners,currentTask))->post();
  fakeProgress = new FakeProgress(currentTask,5000);
};

void ProgressNotifier::taskEnded(ProgressTask * ){
  fakeProgress = nullptr;
  currentTask = nullptr;
}
void ProgressNotifier::taskProgress(ProgressTask *,float p) {
  (new ProgressCallbackMessage(&progressListeners,currentTask,p))->post();
}




ScopedPointer<FakeProgress> fakeProgress;






