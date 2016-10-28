/*
 ==============================================================================

 ProgressNotifier.h
 Created: 6 Oct 2016 2:36:31pm
 Author:  Martin Hermant

 ==============================================================================
 */

#ifndef PROGRESSNOTIFIER_H_INCLUDED
#define PROGRESSNOTIFIER_H_INCLUDED
#include "JuceHeader.h"


class FakeProgress;
class StartCallbackMessage;

class ProgressTask {
  public :
  ProgressTask(String _taskName,ProgressTask * _parentTask = nullptr);
  virtual ~ProgressTask();

  int getTaskPositionInParent();

  float progress;
  String taskName;

  StringArray getAddress();
  ProgressTask * addTask(String taskName);
  void start();
  void end();
  void setProgress(float _progress);


  float getNormalizedProgress();
  void clearTasks();

  int getTaskPosition();



  // internal function for taskTree
  bool isASubTaskOf(ProgressTask * pt);
  ProgressTask * getRootTask();
  ProgressTask * getNextSibling();;
  ProgressTask * getLeftMostLeaf();
  int getTotalNumberOfTasks();
  bool isLeaf();

  ProgressTask * parentTask ;
  OwnedArray<ProgressTask> subTasks;

  class TaskListener{
  public:
    virtual ~TaskListener(){};
    virtual void taskAdded(ProgressTask *){};
    virtual void taskProgress(ProgressTask *,float){};
    virtual void taskStarted(ProgressTask *){};
    virtual void taskEnded(ProgressTask *){};
  };
  ListenerList<TaskListener> taskListeners;
  void addTaskListener(TaskListener * l){taskListeners.add(l);}
  void removeTaskListener(TaskListener * l){taskListeners.remove(l);}



  friend class WeakReference<ProgressTask>;
  WeakReference<ProgressTask>::Master masterReference;
};

// this defer calls from ProgressTask for safe notification
class ProgressNotifier : public ProgressTask, ProgressTask::TaskListener {

public:
  ProgressNotifier();
  virtual ~ProgressNotifier();


  ProgressTask * currentTask;
  void taskAdded(ProgressTask * t) override;
  void taskStarted(ProgressTask * t)override;
  void taskEnded(ProgressTask * )override;
  void taskProgress(ProgressTask * t,float p) override;

  
  class ProgressListener {
    public :

    ProgressListener(ProgressNotifier * notifier=nullptr):baseNotifier(notifier){
      if(notifier){notifier->addListener(this);}
    }
    virtual ~ProgressListener(){if(baseNotifier)baseNotifier->removeListener(this);}

    virtual void startedProgress(ProgressTask *) {};
    virtual void endedProgress(ProgressTask *) {};
    virtual void newProgress(ProgressTask *,float) {};
    virtual void totalNumberOfTaskChanged() {};


    // used for easy handling if only one notifier linked
    ProgressNotifier *  baseNotifier;

  };


  ListenerList<ProgressListener> progressListeners;
  void addListener(ProgressListener* l){progressListeners.add(l);}
  void removeListener(ProgressListener* l){progressListeners.remove(l);}

  ScopedPointer<FakeProgress> fakeProgress;





};




#endif  // PROGRESSNOTIFIER_H_INCLUDED
