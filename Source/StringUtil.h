/*
 ==============================================================================

 StringUtil.h
 Created: 8 Mar 2016 2:43:25pm
 Author:  bkupe

 ==============================================================================
 */

#ifndef STRINGUTIL_H_INCLUDED
#define STRINGUTIL_H_INCLUDED
#pragma once

class CommandLineElement{
public:
  CommandLineElement(const String & name=String::empty):command(name){}
  String command;
  StringArray args;

  bool isEmpty(){return (command == "") && (args.size()==0);}
  CommandLineElement & operator=(const CommandLineElement & rhs)
  {
    if(this == &rhs)
      return *this;
    command = rhs.command;
    args = rhs.args;
    return *this;
  }
  operator bool(){
    return !isEmpty();
  }

};

//typedef Array<CommandLineElement> CommandLineElements;
class CommandLineElements: public Array<CommandLineElement>{
public:

  CommandLineElement  getCommandLineElement(const String command){
    for(auto & e:*this ){
      if(e.command == command){
        return e;
      }
    }
    return CommandLineElement();
  }

  CommandLineElement  getCommandLineElement(const String command, const String command2 ){
    for(auto & e:*this ){
      if((e.command == command) || (e.command==command2)){
        return e;
      }
    }
    return CommandLineElement();
  }

};


class StringUtil
{
public:
  static String toShortName(const String &niceName, bool replaceSlashes = false) {
    if (niceName.isEmpty()) return "";


    String res = niceName;
    if(replaceSlashes) res = res.replaceCharacter('/','_');
    res=res.replaceCharacter('#','_');
    res=res.replaceCharacter('(','_');
    res=res.replaceCharacter(')','_');
    res=res.replaceCharacter('>','_');
    res=res.replaceCharacter('<','_');

    StringArray sa;
    sa.addTokens(res, false);
    int index = 0;

    for (auto &s : sa.strings)
    {
      if (s.isEmpty()) continue;
	  if (s.toUpperCase() == s)
	  {
		  String lowerFull = s.toLowerCase();
		  s.swapWith(lowerFull);
	  } else
	  {
		  String initial = s.substring(0, 1);
		  String upperCaseWord = s.replaceSection(0, 1, index == 0 ? initial.toLowerCase() : initial.toUpperCase());
		  s.swapWith(upperCaseWord);
		  index++;
	  }
     
    }

    return sa.joinIntoString("");
  }




  static CommandLineElements parseCommandLine(const String & commandLine){
    CommandLineElements res;
    StringArray args;
    args.addTokens (commandLine, true);
    args.trim();

    int parsingIdx=0;

    while(parsingIdx<args.size())
    {
      String command = "";
      bool isParameter = args[parsingIdx].startsWith("-");
      if(isParameter){
        command = args[parsingIdx].substring(1, args[parsingIdx].length());
        parsingIdx++;
        res.add(CommandLineElement(command));
        // handles command only args
        if(parsingIdx>=args.size()){break;}
      }

      String argument = args[parsingIdx].removeCharacters(juce::StringRef("\""));

      // handles no command args at the begining
      if (res.size()==0){res.add(CommandLineElement());}

      (res.end()-1)->args.add(argument);
      //DBG("parsing commandline, command : " << command << ", argument :" << argument << " / parsingIdx : " << parsingIdx);

      parsingIdx++;
    }
    return res;

  };
};

#endif  // STRINGUTIL_H_INCLUDED
