/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== conditionScriptTemplate.js ==================
static const unsigned char temp_binary_data_0[] =
"/* ********** GENERAL SCRIPTING **********************\r\n"
"\r\n"
"\t\tThis templates shows what you can do in this is module script\r\n"
"\t\tAll the code outside functions will be executed each time this script is loaded, meaning at file load, when hitting the \"reload\" button or when saving this file\r\n"
"*/\r\n"
"\r\n"
"\r\n"
"// You can add custom parameters to use in your script here, they will be replaced each time this script is saved\r\n"
"var param1 = script.addFloatParameter(\"Param 1\",\"Description of my float param\",.4,0,1); \t\t//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1\r\n"
"var param2 = script.addFloatParameter(\"Param 2\",\"Description of my float param\",.5,0,1); \t\t//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1\r\n"
"\r\n"
"//Here are all the type of parameters you can create\r\n"
"/*\r\n"
"var myTrigger = script.addTrigger(\"My Trigger\", \"Trigger description\"); \t\t\t\t\t\t\t\t\t//This will add a trigger (button)\r\n"
"var myBoolParam = script.addBoolParameter(\"My Bool Param\",\"Description of my bool param\",false); \t\t\t//This will add a boolean parameter (toggle), defaut unchecked\r\n"
"var myFloatParam = script.addFloatParameter(\"My Float Param\",\"Description of my float param\",.1,0,1); \t\t//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1\r\n"
"var myIntParam = script.addIntParameter(\"My Int Param\",\"Description of my int param\",2,0,10); \t\t\t\t//This will add an integer number parameter (stepper), default value of 2, with a range between 0 and 10\r\n"
"var myStringParam = script.addStringParameter(\"My String Param\",\"Description of my string param\", \"cool\");\t//This will add a string parameter (text field), default value is \"cool\"\r\n"
"var myColorParam = script.addColorParameter(\"My Color Param\",\"Description of my color param\",0xff0000ff); \t//This will add a color parameter (color picker), default value of opaque blue (ARGB)\r\n"
"var myP2DParam = script.addPoint2DParameter(\"My P2D Param\",\"Description of my p2d param\"); \t\t\t\t\t//This will add a point 2d parameter\r\n"
"var myP3DParam = script.addPoint3DParameter(\"My P3D Param\",\"Description of my p3d param\"); \t\t\t\t\t//This will add a point 3d parameter\r\n"
"var myTargetParam = script.addTargetParameter(\"My Target Param\",\"Description of my target param\"); \t\t\t//This will add a target parameter (to reference another parameter)\r\n"
"var myEnumParam = script.addEnumParameter(\"My Enum Param\",\"Description of my enum param\",\t\t\t\t\t//This will add a enum parameter (dropdown with options)\r\n"
"\t\t\t\t\t\t\t\t\t\t\t\"Option 1\", 1,\t\t\t\t\t\t\t\t\t\t\t\t\t//Each pair of values after the first 2 arguments define an option and its linked data\r\n"
"\t\t\t\t\t\t\t\t\t\t\t\"Option 2\", 5,\t\t\t\t\t\t\t\t\t\t\t\t    //First argument of an option is the label (string)\r\n"
"\t\t\t\t\t\t\t\t\t\t\t\"Option 3\", \"banana\"\t\t\t\t\t\t\t\t\t\t\t//Second argument is the value, it can be whatever you want\r\n"
"\t\t\t\t\t\t\t\t\t\t\t); \t\r\n"
"*/\r\n"
"\r\n"
"\r\n"
"//you can also declare custom internal variable\r\n"
"//var myValue = 5;\r\n"
"\r\n"
"/*\r\n"
" The init() function will allow you to init everything you want after the script has been checked and loaded\r\n"
" WARNING it also means that if you change values of your parameters by hand and set their values inside the init() function, they will be reset to this value each time the script is reloaded !\r\n"
"*/\r\n"
"function init()\r\n"
"{\r\n"
"\t//myFloatParam.set(5); //The .set() function set the parameter to this value.\r\n"
"\t//myColorParam.set([1,.5,1,1]);\t//for a color parameter, you need to pass an array with 3 (RGB) or 4 (RGBA) values.\r\n"
"\t//myP2DParam.set([1.5,-5]); // for a Point2D parameter, you need to pass 2 values (XY)\r\n"
"\t//myP3DParam.set([1.5,2,-3]); // for a Point3D parameter, you need to pass 3 values (XYZ)\r\n"
"}\r\n"
"\r\n"
"/*\r\n"
" This function will be called each time a parameter of your script has changed\r\n"
"*/\r\n"
"function scriptParameterChanged(param)\r\n"
"{\r\n"
"\t//You can use the script.log() function to show an information inside the logger panel. To be able to actuallt see it in the logger panel, you will have to turn on \"Log\" on this script.\r\n"
"\tscript.log(\"Parameter changed : \"+param.name); //All parameters have \"name\" property\r\n"
"\tif(param.is(myTrigger)) script.log(\"Trigger !\"); //You can check if two variables are the reference to the same parameter or object with the method .is()\r\n"
"\telse if(param.is(myEnumParam)) script.log(\"Label = \"+param.get()+\", data = \"+param.getData()); //The enum parameter has a special function getData() to get the data associated to the option\r\n"
"\telse script.log(\"Value is \"+param.get()); //All parameters have a get() method that will return their value\r\n"
"\r\n"
"\r\n"
"\t//Condition check and validate\r\n"
"\tif(param.is(param1) || param.is(param2))\r\n"
"\t{\r\n"
"\t\tif(param1.get() > param2.get()) local.setValid(true);\r\n"
"\t\telse local.setValid(false);\r\n"
"\t}\r\n"
"}\r\n"
"\r\n"
"/*\r\n"
" This function, if you declare it, will launch a timer at 50hz, calling this method on each tick\r\n"
"*/\r\n"
"/*\r\n"
"function update(deltaTime)\r\n"
"{\r\n"
"\tscript.log(\"Update : \"+util.getTime()+\", delta = \"+deltaTime); //deltaTime is the time between now and last update() call, util.getTime() will give you a timestamp relative to either the launch time of the software, or the start of the computer"
".\r\n"
"}\r\n"
"*/\r\n"
"\r\n"
"/* ********** CONDITION SPECIFIC SCRIPTING **********************\r\n"
"\r\n"
"\tThe \"local\" variable refers to the object containing the scripts. In this case, the local variable refers to the condition.\r\n"
"\tIt means that you can access any control inside  this condition by accessing it through its address.\r\n"
"\tThe condition has a method .setValid() which validate or invalidate this condition.\r\n"
"\tYou can check its usage in the scriptParameterChanged\r\n"
"*/";

const char* conditionScriptTemplate_js = (const char*) temp_binary_data_0;

//================== filterScriptTemplate.js ==================
static const unsigned char temp_binary_data_1[] =
"/* ********** FILTER SPECIFIC SCRIPTING **********************\r\n"
"\r\n"
"\tThe \"local\" variable refers to the object containing the scripts. In this case, the local variable refers to the filter.\r\n"
"\tIt means that you can access any control inside  this filter by accessing it through its address.\r\n"
"*/\r\n"
"\r\n"
"/*\r\n"
" This function will be called each time the filter is processed, and expects a return value.\r\n"
" This function only exists because the script is in a filter\r\n"
"*/\r\n"
"function filter(inputValue, min, max)\r\n"
"{\r\n"
"\tvar result = inputValue * myFloatParam.get(); //Basic multiplication of the input value by the script parameter myFloatParam\r\n"
"\treturn result;\r\n"
"}";

const char* filterScriptTemplate_js = (const char*) temp_binary_data_1;

//================== genericScriptTemplate.js ==================
static const unsigned char temp_binary_data_2[] =
"/* ********** GENERAL SCRIPTING **********************\r\n"
"\r\n"
"\t\tThis templates shows what you can do in this is module script\r\n"
"\t\tAll the code outside functions will be executed each time this script is loaded, meaning at file load, when hitting the \"reload\" button or when saving this file\r\n"
"*/\r\n"
"\r\n"
"\r\n"
"// You can add custom parameters to use in your script here, they will be replaced each time this script is saved\r\n"
"var myFloatParam = script.addFloatParameter(\"My Float Param\",\"Description of my float param\",.1,0,1); \t\t//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1\r\n"
"\r\n"
"//Here are all the type of parameters you can create\r\n"
"/*\r\n"
"var myTrigger = script.addTrigger(\"My Trigger\", \"Trigger description\"); \t\t\t\t\t\t\t\t\t//This will add a trigger (button)\r\n"
"var myBoolParam = script.addBoolParameter(\"My Bool Param\",\"Description of my bool param\",false); \t\t\t//This will add a boolean parameter (toggle), defaut unchecked\r\n"
"var myFloatParam = script.addFloatParameter(\"My Float Param\",\"Description of my float param\",.1,0,1); \t\t//This will add a float number parameter (slider), default value of 0.1, with a range between 0 and 1\r\n"
"var myIntParam = script.addIntParameter(\"My Int Param\",\"Description of my int param\",2,0,10); \t\t\t\t//This will add an integer number parameter (stepper), default value of 2, with a range between 0 and 10\r\n"
"var myStringParam = script.addStringParameter(\"My String Param\",\"Description of my string param\", \"cool\");\t//This will add a string parameter (text field), default value is \"cool\"\r\n"
"var myColorParam = script.addColorParameter(\"My Color Param\",\"Description of my color param\",0xff0000ff); \t//This will add a color parameter (color picker), default value of opaque blue (ARGB)\r\n"
"var myP2DParam = script.addPoint2DParameter(\"My P2D Param\",\"Description of my p2d param\"); \t\t\t\t\t//This will add a point 2d parameter\r\n"
"var myP3DParam = script.addPoint3DParameter(\"My P3D Param\",\"Description of my p3d param\"); \t\t\t\t\t//This will add a point 3d parameter\r\n"
"var myTargetParam = script.addTargetParameter(\"My Target Param\",\"Description of my target param\"); \t\t\t//This will add a target parameter (to reference another parameter)\r\n"
"var myEnumParam = script.addEnumParameter(\"My Enum Param\",\"Description of my enum param\",\t\t\t\t\t//This will add a enum parameter (dropdown with options)\r\n"
"\t\t\t\t\t\t\t\t\t\t\t\"Option 1\", 1,\t\t\t\t\t\t\t\t\t\t\t\t\t//Each pair of values after the first 2 arguments define an option and its linked data\r\n"
"\t\t\t\t\t\t\t\t\t\t\t\"Option 2\", 5,\t\t\t\t\t\t\t\t\t\t\t\t    //First argument of an option is the label (string)\r\n"
"\t\t\t\t\t\t\t\t\t\t\t\"Option 3\", \"banana\"\t\t\t\t\t\t\t\t\t\t\t//Second argument is the value, it can be whatever you want\r\n"
"\t\t\t\t\t\t\t\t\t\t\t); \t\r\n"
"*/\r\n"
"\r\n"
"\r\n"
"//you can also declare custom internal variable\r\n"
"//var myValue = 5;\r\n"
"\r\n"
"/*\r\n"
" The init() function will allow you to init everything you want after the script has been checked and loaded\r\n"
" WARNING it also means that if you change values of your parameters by hand and set their values inside the init() function, they will be reset to this value each time the script is reloaded !\r\n"
"*/\r\n"
"function init()\r\n"
"{\r\n"
"\t//myFloatParam.set(5); //The .set() function set the parameter to this value.\r\n"
"\t//myColorParam.set([1,.5,1,1]);\t//for a color parameter, you need to pass an array with 3 (RGB) or 4 (RGBA) values.\r\n"
"\t//myP2DParam.set([1.5,-5]); // for a Point2D parameter, you need to pass 2 values (XY)\r\n"
"\t//myP3DParam.set([1.5,2,-3]); // for a Point3D parameter, you need to pass 3 values (XYZ)\r\n"
"}\r\n"
"\r\n"
"/*\r\n"
" This function will be called each time a parameter of your script has changed\r\n"
"*/\r\n"
"function scriptParameterChanged(param)\r\n"
"{\r\n"
"\t//You can use the script.log() function to show an information inside the logger panel. To be able to actuallt see it in the logger panel, you will have to turn on \"Log\" on this script.\r\n"
"\tscript.log(\"Parameter changed : \"+param.name); //All parameters have \"name\" property\r\n"
"\tif(param.is(myTrigger)) script.log(\"Trigger !\"); //You can check if two variables are the reference to the same parameter or object with the method .is()\r\n"
"\telse if(param.is(myEnumParam)) script.log(\"Label = \"+param.get()+\", data = \"+param.getData()); //The enum parameter has a special function getData() to get the data associated to the option\r\n"
"\telse script.log(\"Value is \"+param.get()); //All parameters have a get() method that will return their value\r\n"
"}\r\n"
"\r\n"
"/*\r\n"
" This function, if you declare it, will launch a timer at 50hz, calling this method on each tick\r\n"
"*/\r\n"
"/*\r\n"
"function update(deltaTime)\r\n"
"{\r\n"
"\tscript.log(\"Update : \"+util.getTime()+\", delta = \"+deltaTime); //deltaTime is the time between now and last update() call, util.getTime() will give you a timestamp relative to either the launch time of the software, or the start of the computer"
".\r\n"
"}\r\n"
"*/\r\n"
"\r\n";

const char* genericScriptTemplate_js = (const char*) temp_binary_data_2;

//================== midiScriptTemplate.js ==================
static const unsigned char temp_binary_data_3[] =
"/* ********** MIDI MODULE SPECIFIC SCRIPTING ********************* */\r\n"
"/*\r\n"
"\r\n"
"MIDI Modules have specific methods that can be used to send MIDI events such as noteOn, noteOff, controlChange and sysEx messages from Script.\r\n"
"If you want to send a MIDI event from this script, you can do the following :\r\n"
"\r\n"
"local.sendNoteOn(1, 12, 127); //This will send a NoteOn Event on channel 1, pitch 12, velocity 127\r\n"
"local.sendNoteOff(1, 12); //This will send a NoteOff Event on chanenl 1, pitch 12\r\n"
"local.sendCC(3, 20, 65); //This will send a ControlChange on channel 3, number 20, value 65\r\n"
"local.sendSysEx(15,20,115,10); //This will send 4 bytes as a SysEx message\r\n"
"*/\r\n"
"\r\n"
"/*\r\n"
"You can intercept MIDI Events with the functions below\r\n"
"*/\r\n"
"\r\n"
"function noteOnEvent(channel, pitch, velocity)\r\n"
"{\r\n"
"\tscript.log(\"Note on received \"+channel+\", \"+pitch+\", \"+velocity);\r\n"
"}\r\n"
"\r\n"
"\r\n"
"function noteOffEvent(channel, pitch, velocity)\r\n"
"{\r\n"
"\tscript.log(\"Note off received \"+channel+\", \"+pitch+\", \"+velocity);\r\n"
"}\r\n"
"\r\n"
"function ccEvent(channel, number, value)\r\n"
"{\r\n"
"\tscript.log(\"ControlChange received \"+channel+\", \"+number+\", \"+value);\r\n"
"}\r\n"
"\r\n"
"function sysExEvent(data)\r\n"
"{\r\n"
"\tscript.log(\"Sysex Message received, \"+data.length+\" bytes :\");\r\n"
"\tfor(var i=0; i < data.length; i++)\r\n"
"\t{\r\n"
"\t\tscript.log(\" > \"+data[i]);\r\n"
"\t}\r\n"
"}";

const char* midiScriptTemplate_js = (const char*) temp_binary_data_3;

//================== moduleScriptTemplate.js ==================
static const unsigned char temp_binary_data_4[] =
"/* ********** MODULE SPECIFIC SCRIPTING **********************\r\n"
"\r\n"
"\tThe \"local\" variable refers to the object containing the scripts. In this case, the local variable refers to the module.\r\n"
"\tIt means that you can access any control inside  this module by accessing it through its address.\r\n"
"\tFor instance, if the module has a float value named \"Density\", you can access it via local.values.density\r\n"
"\tThen you can retrieve its value using local.values.density.get() and change its value using local.values.density.set()\r\n"
"*/\r\n"
"\r\n"
"/*\r\n"
" This function will be called each time a parameter of this module has changed, meaning a parameter or trigger inside the \"Parameters\" panel of this module\r\n"
" This function only exists because the script is in a module\r\n"
"*/\r\n"
"function moduleParameterChanged(param)\r\n"
"{\r\n"
"\tif(param.isParameter())\r\n"
"\t{\r\n"
"\t\tscript.log(\"Module parameter changed : \"+param.name+\" > \"+param.get());\r\n"
"\t}else \r\n"
"\t{\r\n"
"\t\tscript.log(\"Module parameter triggered : \"+value.name);\t\r\n"
"\t}\r\n"
"}\r\n"
"\r\n"
"/*\r\n"
" This function will be called each time a value of this module has changed, meaning a parameter or trigger inside the \"Values\" panel of this module\r\n"
" This function only exists because the script is in a module\r\n"
"*/\r\n"
"function moduleValueChanged(value)\r\n"
"{\r\n"
"\tif(value.isParameter())\r\n"
"\t{\r\n"
"\t\tscript.log(\"Module value changed : \"+value.name+\" > \"+value.get());\t\r\n"
"\t}else \r\n"
"\t{\r\n"
"\t\tscript.log(\"Module value triggered : \"+value.name);\t\r\n"
"\t}\r\n"
"}";

const char* moduleScriptTemplate_js = (const char*) temp_binary_data_4;

//================== oscScriptTemplate.js ==================
static const unsigned char temp_binary_data_5[] =
"/* ********** OSC MODULE SPECIFIC SCRIPTING ********************* */\r\n"
"/*\r\n"
"\r\n"
"OSC Modules have specific methods that can be used to send OSC message from Script.\r\n"
"If you want to send an OSC Message from this script, you can do the following :\r\n"
"\r\n"
"local.send(\"/myAddress\",1,.5f,\"cool\"); //This will send an OSC Message with address \"/myAddress\" and 3 arguments <int>, <float> and <string>\r\n"
"\r\n"
"*/\r\n"
"/*\r\n"
"You can intercept OSC message with the function oscEvent(address, args)\r\n"
"*/\r\n"
"\r\n"
"function oscEvent(address, args)\r\n"
"{\r\n"
"\t//param \"address\" is the address of the OSC Message\r\n"
"\t//param \"args\" is an array containing all the arguments of the OSC Message\r\n"
"\r\n"
"\tscript.log(\"OSC Message received \"+address+\", \"+args.length+\" arguments\");\r\n"
"\tfor(var i=0; i < args.length; i++)\r\n"
"\t{\r\n"
"\t\tscript.log(\" > \"+args[i]);\r\n"
"\t}\r\n"
"}";

const char* oscScriptTemplate_js = (const char*) temp_binary_data_5;

//================== streamingScriptTemplate.js ==================
static const unsigned char temp_binary_data_6[] =
"/* ********** STREAMING MODULE (UDP, SERIAL) SPECIFIC SCRIPTING ********************* */\r\n"
"/*\r\n"
"\r\n"
"Streaming Modules (i.e. UDP and Serial Module) have specific methods that can be used to handle receiving and sendin data over the connection.\r\n"
"With streaming modules, there are 2 ways of sending data : either as a UTF-8 String or as separate bytes\r\n"
"\r\n"
"local.send(\"This is my message\"); //This will send the string passed in as ASCII characters \r\n"
"local.sendBytes(30,210,46,255,10); //This will send all the bytes passed in as they are\r\n"
"\r\n"
"*/\r\n"
"\r\n"
"/*\r\n"
"You can intercept all the received data from this module with the method dataReceived(data).\r\n"
"Depending on the Protocol you chose, the nature of the data passed in this function will be different.\r\n"
"*/\r\n"
"\r\n"
"function dataReceived(data)\r\n"
"{\r\n"
"\t//If mode is \"Lines\", you can expect data to be a single line String\r\n"
"\tscript.log(\"Data received : \" +data);\r\n"
"\r\n"
"\t//If mode is anything else, you can expect data to be an array of bytes\r\n"
"\tscript.log(\"Bytes received : \"+data.length);\r\n"
"\tfor(var i=0; i < data.length; i++)\r\n"
"\t{\r\n"
"\t\tscript.log(\" > \" + data[i]);\r\n"
"\t}\r\n"
"}";

const char* streamingScriptTemplate_js = (const char*) temp_binary_data_6;

//================== about.png ==================
static const unsigned char temp_binary_data_7[] =
{ 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,3,32,0,0,1,224,8,6,0,0,0,93,7,9,245,0,0,0,9,112,72,89,115,0,0,14,196,0,0,14,196,1,149,43,14,27,0,0,6,233,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,
97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,
111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,50,32,55,57,46,49,54,48,57,50,52,44,32,50,48,49,55,47,48,55,47,49,51,45,48,49,58,48,54,58,51,57,
32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,
35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,
48,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,
115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,
100,111,98,101,46,99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,
109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,87,105,110,100,111,119,115,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,56,45,48,49,45,48,52,
84,50,48,58,48,50,58,51,53,43,48,49,58,48,48,34,32,120,109,112,58,77,101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,56,45,48,49,45,48,52,84,50,48,58,48,50,58,51,53,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,61,
34,50,48,49,56,45,48,49,45,48,52,84,50,48,58,48,50,58,51,53,43,48,49,58,48,48,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,102,97,52,50,56,101,48,45,51,50,55,100,45,54,57,52,57,45,97,48,56,55,45,
97,99,102,101,50,55,57,97,101,99,52,52,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,48,99,54,100,97,99,51,101,45,99,55,102,99,45,50,48,52,48,45,
97,54,99,57,45,102,100,51,99,54,101,98,50,97,102,54,57,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,100,56,57,48,100,50,56,55,45,51,51,99,99,45,97,98,52,52,45,97,49,55,
55,45,99,102,55,52,100,100,50,100,56,55,101,54,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,62,32,60,120,109,112,77,77,58,72,
105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,
46,105,105,100,58,100,56,57,48,100,50,56,55,45,51,51,99,99,45,97,98,52,52,45,97,49,55,55,45,99,102,55,52,100,100,50,100,56,55,101,54,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,56,45,48,49,45,48,52,84,50,48,58,48,50,58,51,53,43,48,49,58,
48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,87,105,110,100,111,119,115,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,
58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,57,102,97,52,50,56,101,48,45,51,50,55,100,45,54,57,52,57,45,97,48,56,55,45,97,99,102,101,50,55,57,97,
101,99,52,52,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,56,45,48,49,45,48,52,84,50,48,58,48,50,58,51,53,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,
111,116,111,115,104,111,112,32,67,67,32,40,87,105,110,100,111,119,115,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,47,114,100,102,58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,112,
104,111,116,111,115,104,111,112,58,84,101,120,116,76,97,121,101,114,115,62,32,60,114,100,102,58,66,97,103,62,32,60,114,100,102,58,108,105,32,112,104,111,116,111,115,104,111,112,58,76,97,121,101,114,78,97,109,101,61,34,67,104,97,116,97,105,103,110,101,
32,105,115,32,97,110,32,111,112,101,110,45,115,111,117,114,99,101,32,115,111,102,116,119,97,114,101,44,32,32,105,102,32,121,111,117,32,119,97,110,116,32,116,111,32,115,117,112,112,111,114,116,32,109,34,32,112,104,111,116,111,115,104,111,112,58,76,97,
121,101,114,84,101,120,116,61,34,67,104,97,116,97,105,103,110,101,32,105,115,32,97,110,32,111,112,101,110,45,115,111,117,114,99,101,32,115,111,102,116,119,97,114,101,44,32,32,105,102,32,121,111,117,32,119,97,110,116,32,116,111,32,115,117,112,112,111,
114,116,32,109,101,44,32,121,111,117,32,99,97,110,32,109,97,107,101,32,97,32,100,111,110,97,116,105,111,110,32,102,114,111,109,32,116,104,101,32,119,101,98,115,105,116,101,32,33,34,47,62,32,60,47,114,100,102,58,66,97,103,62,32,60,47,112,104,111,116,111,
115,104,111,112,58,84,101,120,116,76,97,121,101,114,115,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,107,101,116,32,101,
110,100,61,34,114,34,63,62,227,243,164,225,0,0,137,40,73,68,65,84,120,218,237,221,7,120,84,85,250,199,241,59,147,222,51,147,73,0,233,161,131,128,244,222,123,179,32,168,8,98,195,128,244,18,8,189,88,177,173,107,151,181,252,119,109,187,98,219,93,219,186,
184,186,174,109,85,116,45,171,238,170,216,203,218,176,162,32,224,251,63,229,222,100,8,41,51,147,4,98,248,158,231,249,60,9,201,148,59,119,102,194,249,205,57,239,57,142,136,56,0,0,0,0,112,32,112,18,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,
0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,
0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,4,16,78,2,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,1,4,0,
0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,
1,4,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,
0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,78,2,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,
0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,194,73,0,0,0,0,64,0,1,
0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,4,16,28,192,39,131,86,83,45,65,169,167,180,85,250,43,19,148,147,148,51,149,197,202,82,101,185,178,162,148,149,
137,234,119,25,126,231,140,134,241,206,184,166,9,78,151,20,191,147,155,228,115,124,9,62,199,25,144,226,56,173,212,5,212,183,78,107,245,85,253,220,105,169,190,118,73,118,156,250,241,142,211,33,201,113,210,252,86,3,245,239,198,9,246,247,250,178,35,210,
237,101,66,74,102,156,58,56,245,53,81,93,191,79,170,227,156,17,116,156,56,245,125,103,117,59,227,50,236,101,107,19,222,171,0,0,128,0,66,0,33,96,168,190,189,210,91,57,202,13,21,215,40,127,81,254,169,188,172,124,172,252,160,72,21,236,85,62,84,254,165,60,
20,138,115,46,204,244,59,147,146,125,206,17,45,18,157,20,29,64,218,169,208,209,59,197,6,10,2,8,0,0,0,1,132,0,242,203,111,1,165,131,59,130,177,90,185,94,121,82,249,92,217,93,86,112,200,72,244,73,78,138,79,26,103,248,165,115,110,188,12,110,156,32,71,183,
74,148,105,29,146,100,230,17,201,50,167,75,178,44,233,145,34,171,250,236,171,168,87,138,204,82,191,63,182,117,162,244,105,24,47,205,178,252,18,72,246,137,223,183,223,125,236,84,158,81,46,85,65,99,82,243,4,167,81,51,21,54,142,72,38,128,0,0,0,16,64,8,32,
191,164,166,186,234,78,119,101,138,114,158,242,168,242,142,242,99,233,144,161,3,70,143,6,241,114,140,10,11,167,118,74,146,115,6,164,202,109,19,210,229,169,105,89,242,214,25,217,242,254,172,128,124,62,55,32,63,21,230,136,172,80,214,132,68,214,230,138,
172,87,95,215,231,90,27,114,75,190,47,22,178,151,93,158,35,219,23,4,229,221,51,3,242,175,83,178,229,166,113,233,178,176,123,178,140,111,153,40,205,85,48,41,117,60,159,102,196,57,119,5,226,156,147,178,227,156,220,116,191,13,31,4,16,0,0,0,2,8,1,164,118,
53,213,221,118,186,42,179,148,235,148,167,220,209,133,226,206,125,122,162,79,218,4,227,100,120,179,120,51,66,113,237,168,116,249,211,177,25,242,134,10,25,59,23,187,225,98,181,10,12,235,66,37,129,98,157,178,214,13,18,171,148,21,54,80,72,145,178,44,2,250,
178,43,67,246,118,215,134,74,194,138,190,61,245,243,47,231,7,228,129,201,153,114,174,10,61,3,27,37,72,154,58,198,176,99,254,175,202,22,23,169,48,210,177,153,10,30,109,92,4,16,0,0,64,0,1,1,228,224,180,195,149,153,202,141,202,91,110,157,69,113,7,190,113,
166,95,38,182,73,148,21,125,82,228,230,113,233,242,166,10,26,223,45,14,170,32,145,99,67,192,6,55,96,172,46,39,88,44,173,1,133,110,32,57,75,221,239,57,174,243,148,243,243,204,207,190,94,24,148,223,170,99,29,215,50,49,60,136,236,74,246,57,183,53,136,119,
250,181,77,114,156,163,50,237,180,44,2,8,0,0,32,128,128,0,82,179,45,213,45,22,63,71,121,88,249,38,60,112,180,200,142,147,113,45,18,229,215,195,210,100,203,241,153,242,209,236,128,234,236,231,148,140,104,132,135,141,154,10,24,149,133,143,85,33,249,89,
221,247,63,79,202,146,171,71,166,201,130,238,41,114,82,135,36,153,214,62,201,140,202,252,225,200,12,121,115,102,182,124,57,63,40,47,158,146,45,11,186,165,152,58,20,47,136,36,249,156,27,123,164,56,45,117,232,200,35,128,0,0,0,2,8,8,32,213,222,50,149,99,
149,171,149,255,132,7,142,220,20,159,76,108,157,36,151,13,79,147,127,78,203,146,31,10,131,54,108,156,157,107,191,234,41,83,53,61,162,17,13,117,60,31,207,13,202,224,38,9,149,174,154,213,58,24,39,171,250,166,200,43,167,102,203,182,89,1,57,103,80,170,36,
197,21,255,254,107,21,68,214,170,224,145,152,69,0,1,0,0,4,16,16,64,170,220,146,148,193,202,149,165,67,71,187,156,56,153,125,68,178,60,116,188,14,28,57,118,42,211,5,121,34,231,186,83,170,214,122,5,226,110,157,197,50,119,228,225,96,135,15,29,132,212,177,
45,237,153,28,213,242,189,42,76,200,194,238,41,242,193,188,128,124,48,39,104,70,75,188,223,249,29,231,177,52,191,211,189,33,1,4,0,0,16,64,64,0,137,169,245,80,206,87,94,11,239,132,119,173,31,39,43,251,164,200,63,167,103,217,105,84,231,229,201,247,139,
115,228,238,137,153,102,25,220,9,45,19,101,68,179,4,25,210,36,65,70,55,79,144,73,109,18,229,236,129,169,242,242,105,217,118,202,85,81,45,8,32,203,109,33,186,94,194,55,150,125,68,210,19,124,114,253,216,116,83,51,242,224,9,153,210,73,5,49,247,119,63,101,
250,157,37,122,115,195,190,4,16,0,0,64,0,1,1,164,210,150,165,76,83,238,213,157,233,240,41,72,122,169,218,135,85,103,123,175,238,192,235,81,142,141,121,178,245,228,108,179,7,71,48,217,87,105,167,61,51,201,39,183,29,153,97,71,69,14,118,0,209,35,49,42,60,
221,59,49,163,42,155,25,202,76,21,184,116,0,251,162,48,71,78,57,60,41,252,119,55,13,73,115,82,22,134,28,199,71,0,1,0,0,4,16,16,64,246,107,157,220,253,57,222,246,58,209,245,211,252,114,74,199,36,185,247,216,76,249,118,81,208,78,169,210,245,28,231,170,
224,113,74,182,25,233,136,182,195,222,52,211,47,223,45,9,218,122,144,218,16,66,86,135,204,210,191,167,170,199,185,164,103,138,220,126,84,186,60,48,41,67,238,87,238,62,38,67,214,245,79,149,254,141,226,43,124,76,215,142,72,51,97,76,239,51,114,253,184,116,
51,58,162,127,30,136,115,254,209,54,201,105,162,87,199,34,128,0,0,0,2,8,8,32,182,245,87,126,171,124,239,117,168,187,213,143,151,75,134,164,153,141,250,76,221,198,89,238,106,85,171,236,222,25,191,26,150,22,190,18,84,84,114,83,125,242,221,194,90,18,64,
10,221,90,144,117,238,190,32,107,66,37,203,1,123,214,219,186,149,191,76,201,52,211,202,202,122,76,39,31,158,84,178,47,137,10,103,255,152,150,37,13,51,138,55,52,252,151,10,34,13,186,165,216,229,122,71,171,16,50,74,117,252,71,214,18,188,87,1,0,0,1,132,
0,114,160,218,49,202,253,225,53,13,211,84,71,90,111,194,183,119,121,78,73,232,40,10,43,218,94,25,146,229,189,83,34,10,26,217,201,126,201,75,245,135,175,20,101,92,62,60,173,118,76,193,42,171,40,125,89,57,163,36,43,220,16,166,66,198,141,227,210,37,43,105,
223,240,165,71,136,76,45,140,119,121,117,217,247,230,4,164,91,189,226,145,147,255,180,78,114,90,233,240,49,56,205,113,134,42,195,210,107,7,222,171,0,0,128,0,66,0,169,233,54,90,121,192,235,60,231,164,248,204,170,78,207,159,234,22,136,235,79,253,203,26,
157,88,23,146,199,167,102,85,24,58,70,53,79,52,1,227,175,199,103,202,107,51,178,229,237,153,1,121,106,90,150,252,118,108,186,92,56,56,85,117,212,51,74,54,24,92,250,11,227,109,88,120,78,174,121,76,227,91,38,72,255,70,9,114,253,232,116,187,183,201,242,
82,231,236,108,21,66,230,6,228,136,188,56,111,245,172,23,3,241,78,118,94,156,227,28,158,228,56,93,146,237,180,172,131,141,247,42,0,0,32,128,16,64,106,170,13,85,238,243,194,66,163,12,191,217,215,226,61,61,205,234,44,119,83,192,138,86,167,82,193,228,246,
35,211,203,12,30,199,182,78,148,39,84,167,92,86,231,186,183,229,78,105,90,109,167,108,153,80,19,126,31,133,191,192,0,18,110,109,168,100,3,197,245,21,60,38,245,184,63,153,23,144,246,57,118,36,196,239,115,254,26,138,115,18,117,0,57,66,117,254,59,213,2,
188,87,1,0,0,1,132,0,82,221,173,141,114,171,23,22,244,180,168,117,253,82,229,253,217,65,219,121,142,52,20,168,206,246,103,243,131,210,181,100,90,145,36,199,251,228,114,93,128,189,218,157,162,180,50,244,203,14,22,209,46,227,27,201,14,238,103,231,202,171,
51,178,77,237,139,62,103,41,62,231,215,58,124,116,79,177,33,228,96,227,189,10,0,0,8,32,4,144,234,92,78,119,173,242,157,87,227,81,216,43,69,62,156,19,176,43,89,173,14,69,183,27,185,14,40,107,114,229,131,51,3,178,162,79,138,153,182,245,164,30,245,168,203,
193,195,173,123,41,46,80,247,10,205,87,68,113,238,220,154,144,191,159,152,37,113,126,27,66,50,253,206,212,252,68,199,105,167,180,73,58,184,120,175,2,0,0,2,8,1,164,58,218,56,229,5,111,164,226,248,182,73,242,130,222,4,112,189,59,61,106,89,21,58,229,222,
180,170,53,238,215,101,117,96,74,85,120,177,249,154,80,201,200,144,122,172,63,169,199,246,249,188,160,188,175,130,215,135,179,3,242,213,130,160,189,108,248,229,86,149,83,215,162,47,183,220,13,50,42,244,157,55,48,213,27,61,250,188,126,188,211,188,141,
10,32,141,18,172,198,7,9,239,85,0,0,64,0,33,128,84,165,213,87,126,227,5,143,158,13,226,229,222,73,238,10,77,218,178,106,236,172,47,43,167,179,237,46,213,107,58,231,158,117,238,200,193,242,80,245,29,67,117,242,166,144,169,160,240,198,25,217,114,245,168,
52,153,213,57,73,70,55,245,75,235,76,71,178,124,142,36,234,233,83,122,25,225,120,71,186,134,116,221,75,156,44,235,149,34,247,76,204,144,207,230,5,74,150,240,93,30,22,200,86,187,193,100,121,201,72,202,120,119,239,20,213,247,127,176,133,186,209,163,51,
29,231,200,12,199,153,112,144,240,94,5,0,0,4,16,2,72,172,109,172,242,154,87,155,177,182,111,138,124,187,196,93,78,87,127,178,95,19,163,20,225,35,6,107,109,103,251,139,249,1,121,245,244,108,121,122,90,150,252,125,74,166,89,49,74,255,251,115,245,115,89,
145,83,18,78,214,70,57,149,169,6,131,199,151,11,2,242,27,21,58,134,54,116,196,95,214,10,95,137,153,146,144,153,43,113,233,33,113,124,251,47,67,156,167,66,201,201,29,226,229,175,199,101,218,21,177,214,219,13,27,245,102,134,19,90,36,202,227,39,102,217,
251,82,33,240,237,130,128,164,185,123,168,52,140,119,142,93,16,114,156,51,131,7,15,239,85,0,0,64,0,33,128,68,219,226,149,11,188,206,240,144,38,9,118,73,221,179,114,107,110,179,191,85,37,35,6,175,169,112,113,245,200,52,57,161,109,130,116,11,57,146,19,
87,246,18,189,1,191,99,126,127,92,235,4,185,102,84,186,188,162,167,132,121,251,141,172,10,29,184,105,92,222,230,131,234,126,191,93,148,99,150,7,110,144,82,114,156,89,161,250,50,225,168,163,229,87,151,94,42,15,254,229,47,242,207,127,254,83,222,126,251,
109,249,232,163,143,228,131,15,62,144,215,95,127,93,30,251,199,63,228,238,123,238,145,101,69,203,165,71,239,190,226,196,151,132,146,65,141,19,228,177,169,89,242,244,244,44,73,73,176,63,251,221,216,116,59,58,178,212,222,239,175,134,218,169,88,234,137,
123,227,136,20,39,187,79,170,45,74,239,113,16,240,94,5,0,0,4,16,2,72,52,173,153,242,55,221,153,245,41,107,250,166,202,143,110,39,183,70,150,187,93,101,167,25,125,95,24,148,223,141,79,151,17,77,202,8,27,241,233,18,106,212,92,218,116,236,34,29,187,245,
148,182,29,187,154,127,59,9,25,251,239,27,210,196,39,55,140,73,151,29,133,57,229,239,63,82,221,225,99,181,29,129,209,27,46,30,30,44,57,150,62,253,6,200,245,215,95,47,239,189,247,158,68,211,126,254,249,103,121,238,185,231,100,237,218,181,146,215,176,73,
73,144,73,180,95,103,119,77,146,221,222,138,89,238,168,203,78,245,181,179,187,154,88,146,207,41,210,5,233,77,19,28,167,201,65,192,123,21,0,0,16,64,8,32,145,182,9,202,255,116,39,182,69,182,95,30,60,62,179,230,70,19,150,217,128,176,87,125,189,118,84,186,
228,103,134,141,108,228,54,144,99,142,157,36,151,95,126,133,60,251,236,115,178,109,219,54,217,190,125,187,236,222,189,219,116,208,245,87,253,111,253,243,103,159,125,86,174,186,250,106,57,234,152,99,37,84,191,81,241,109,180,200,114,228,178,225,105,230,
246,205,99,168,137,194,118,125,123,107,237,138,93,133,61,75,70,44,122,246,238,35,247,222,123,175,84,71,251,226,139,47,228,156,115,207,149,180,172,96,241,237,223,172,130,154,156,159,183,111,109,200,57,121,242,167,99,51,189,203,124,172,130,64,94,7,189,
42,85,226,129,199,123,21,0,0,16,64,8,32,145,180,211,149,157,186,3,59,162,89,130,108,243,150,214,13,239,228,22,170,159,45,201,182,244,247,133,193,200,130,198,126,187,127,219,80,240,252,41,89,50,176,161,191,184,99,221,177,115,23,21,58,46,151,119,222,121,
39,166,206,186,158,206,164,175,223,165,91,143,146,233,75,234,246,159,60,41,171,100,105,223,194,106,12,31,235,67,242,229,162,160,28,233,22,129,59,190,120,57,251,236,179,101,199,142,29,82,221,237,165,151,94,146,177,99,199,218,29,208,149,95,15,75,179,139,
0,232,81,144,21,110,16,90,147,43,189,15,75,48,151,9,198,57,75,6,167,57,206,128,131,128,247,42,0,0,32,128,16,64,42,107,103,123,29,246,249,93,147,109,71,93,23,61,47,85,95,151,168,160,177,72,117,118,23,166,170,175,25,234,223,89,214,226,76,245,179,52,75,
135,145,165,161,253,59,232,43,66,37,211,160,10,189,125,63,108,145,249,249,131,138,151,143,149,214,109,218,201,205,55,223,44,63,253,244,83,181,116,214,247,238,221,43,191,253,237,111,229,240,78,157,139,239,227,226,193,169,37,251,110,20,86,199,200,71,72,
190,94,146,35,189,27,216,105,79,141,154,52,149,135,31,126,88,106,186,173,95,191,190,248,49,173,234,147,34,114,110,158,89,9,236,152,86,137,178,162,87,138,220,54,62,195,11,41,175,53,78,112,146,245,114,188,13,15,176,67,177,110,234,227,143,63,22,165,200,
161,209,104,180,202,255,94,20,40,219,221,191,27,195,57,35,101,55,250,125,4,16,212,237,206,212,197,94,135,246,34,221,73,215,159,170,235,78,250,98,21,50,22,164,218,112,177,182,133,200,5,221,69,174,24,46,114,245,56,145,107,148,171,70,139,92,210,87,245,136,
91,217,17,17,125,249,240,16,178,34,71,222,157,21,48,59,164,191,116,74,182,13,34,107,115,229,39,245,243,153,71,36,23,119,162,23,44,88,32,159,126,250,105,141,116,214,191,252,242,75,153,61,123,118,241,125,157,220,49,73,118,123,171,102,21,86,173,230,99,103,
81,142,140,110,102,71,27,154,53,107,38,47,191,252,178,28,168,118,245,213,87,23,63,166,235,70,167,203,234,62,54,204,117,8,197,201,87,139,115,164,91,125,27,138,210,252,206,49,1,149,68,50,252,74,220,129,83,151,222,51,170,115,144,175,108,82,182,185,157,5,
109,139,18,168,233,0,162,110,111,178,238,164,84,195,237,108,85,54,255,194,58,104,191,184,99,166,209,34,124,109,15,119,255,94,108,116,191,15,84,247,123,158,0,2,2,8,106,123,0,185,214,124,90,238,247,201,45,19,50,236,148,171,21,65,27,60,116,103,251,130,110,
34,55,76,22,185,125,150,200,157,243,69,238,94,40,114,215,2,215,66,235,206,121,54,140,232,233,88,58,136,120,29,117,21,56,244,74,77,102,73,217,84,191,188,90,16,48,1,100,98,43,59,93,41,45,35,211,140,122,28,136,118,199,157,119,74,114,106,154,185,223,99,90,
38,202,78,111,211,191,88,66,200,114,187,36,238,169,135,39,217,90,147,150,45,229,173,183,222,146,3,221,126,243,155,223,236,83,124,159,149,228,147,39,166,102,138,92,152,39,27,221,209,37,149,5,254,168,87,165,26,156,234,56,253,14,160,186,242,158,113,195,
199,118,55,124,20,185,159,90,106,27,15,196,8,136,123,159,82,13,183,179,169,172,99,174,229,157,180,95,220,49,211,104,85,125,95,87,215,123,158,0,2,2,8,106,115,0,185,72,191,191,147,226,124,114,215,49,153,166,136,89,150,234,81,143,20,21,30,218,138,220,120,
188,10,24,110,232,184,99,174,10,33,103,138,252,97,102,41,179,108,0,185,103,137,200,37,253,68,22,165,151,212,133,172,12,201,199,11,130,166,158,68,223,79,155,64,156,28,229,214,74,4,67,185,242,196,19,79,28,208,14,251,223,255,254,119,9,229,230,217,93,220,
219,36,218,0,18,75,113,189,10,105,119,28,109,167,57,197,39,36,154,37,117,15,86,91,177,98,69,113,0,49,53,33,122,58,214,186,92,121,253,244,108,73,140,51,251,130,124,219,38,209,105,209,41,217,113,116,65,250,129,82,135,2,200,54,247,147,248,64,4,151,21,183,
211,188,217,253,94,95,119,114,57,151,237,230,142,162,136,27,112,10,202,233,128,75,152,109,101,92,102,99,216,200,76,69,247,87,110,56,114,3,213,182,176,227,223,90,214,101,221,142,209,86,247,235,246,242,142,187,212,99,220,90,222,136,81,132,231,179,168,156,
80,232,221,174,254,154,31,197,109,70,115,190,10,194,158,163,114,47,91,193,125,5,220,251,219,30,118,172,195,43,122,172,165,94,19,69,85,57,183,165,158,47,239,49,111,142,246,121,136,244,156,187,143,119,83,216,107,163,220,251,42,253,220,86,210,33,247,158,
11,125,191,91,202,25,77,8,63,111,27,43,120,238,43,125,221,86,240,248,43,124,110,34,125,190,189,199,26,62,154,26,233,123,62,236,186,129,176,251,44,125,46,55,87,240,183,98,123,101,239,199,8,206,119,149,222,215,4,16,16,64,8,32,231,235,247,118,130,234,164,
254,241,88,55,124,20,102,218,105,84,151,14,180,129,67,143,110,232,145,143,253,66,71,105,5,118,116,228,166,147,68,150,215,183,181,33,97,83,149,126,42,202,145,147,59,150,76,185,202,81,225,227,96,117,218,95,120,225,5,9,4,115,204,113,44,235,145,98,235,92,
138,162,152,122,165,66,203,7,179,3,146,151,98,55,253,187,252,138,43,228,96,183,65,131,6,219,169,108,221,146,108,33,250,114,27,172,38,184,97,47,195,239,204,209,251,129,116,77,62,112,234,194,123,38,108,154,196,240,40,58,204,222,180,138,240,206,107,126,
57,183,237,77,191,240,58,29,221,202,153,250,37,238,229,186,149,211,169,152,236,254,126,115,5,247,87,94,103,190,244,84,144,45,21,92,182,40,172,99,85,217,227,243,70,142,54,134,117,222,182,86,83,0,217,230,222,222,240,104,71,156,162,60,95,17,61,151,21,220,
215,22,175,179,91,234,121,30,94,201,253,133,95,182,32,214,115,27,246,124,109,115,143,193,251,247,166,24,131,120,133,231,220,253,253,54,247,252,78,246,194,123,53,5,16,47,60,77,46,163,51,236,253,110,184,167,156,48,191,205,189,252,100,247,58,147,163,120,
252,91,189,64,225,189,126,98,125,190,43,122,95,87,246,158,47,253,55,201,189,175,210,33,102,123,89,207,113,216,107,127,178,23,16,99,56,223,85,126,95,19,64,64,0,57,180,3,200,44,47,12,220,54,33,125,223,240,161,167,82,221,189,200,6,144,74,131,71,24,51,58,
50,203,173,7,73,47,217,159,66,119,218,47,200,147,63,77,116,151,135,245,249,229,254,251,239,63,168,29,246,251,238,187,79,124,113,118,84,230,143,19,221,105,103,81,44,27,60,167,139,13,83,35,70,142,148,218,208,94,121,229,21,73,74,177,83,174,30,159,154,101,
67,149,162,151,32,214,63,75,241,57,247,28,161,66,193,225,73,209,235,228,126,109,175,116,116,255,237,253,172,173,187,228,110,219,50,212,145,0,18,213,84,8,239,63,237,82,255,89,87,58,45,171,172,79,49,99,57,134,74,110,167,188,159,239,211,129,168,232,152,
195,58,176,249,17,92,118,83,248,167,176,97,29,197,225,213,16,64,98,234,68,199,112,190,162,126,46,203,232,40,78,46,163,35,187,165,156,251,219,82,86,167,55,214,115,91,250,249,10,187,238,246,24,206,85,133,231,188,172,176,94,81,128,143,33,128,108,169,32,
244,108,141,96,244,98,159,231,194,189,222,150,88,223,219,213,240,124,199,60,5,43,124,148,199,13,9,222,104,68,32,236,177,14,143,32,136,111,143,225,124,87,249,125,77,0,1,1,228,208,13,32,163,148,159,245,251,250,10,61,101,231,108,119,218,149,174,221,184,
102,172,13,31,101,78,181,138,32,128,252,190,64,100,93,11,213,81,207,176,203,236,234,98,118,229,147,121,65,105,149,109,71,12,206,57,231,156,90,209,105,191,228,146,95,153,227,169,159,234,147,207,23,4,109,81,122,101,1,68,61,150,119,103,6,196,239,211,225,
205,39,91,183,110,149,218,210,138,150,219,169,88,195,26,199,21,239,40,255,234,105,217,146,146,96,206,251,199,105,126,39,148,238,119,156,104,37,248,28,39,47,94,133,138,36,91,200,158,164,36,171,159,229,198,217,29,207,251,167,57,142,222,113,189,180,67,56,
128,20,69,49,245,169,40,172,243,16,83,0,113,59,28,155,75,21,200,71,19,64,246,155,178,18,205,177,84,50,74,177,201,61,190,252,104,59,240,145,28,135,219,137,236,22,195,84,154,152,206,87,148,1,164,204,231,205,237,248,73,132,247,87,222,101,35,58,183,229,60,
95,229,29,87,183,74,70,16,42,60,231,21,220,110,68,231,55,130,0,178,177,178,206,120,5,207,133,55,74,208,45,236,124,109,140,38,136,149,26,77,42,170,134,231,187,42,1,100,179,23,186,220,199,95,60,162,227,77,143,172,96,218,222,230,176,105,88,177,156,239,42,
191,175,9,32,32,128,28,154,1,164,161,254,27,162,223,211,243,244,82,187,235,245,242,173,1,187,140,238,229,195,84,248,88,236,134,143,130,40,3,72,129,157,174,245,219,227,76,231,119,227,128,68,57,170,85,162,60,48,201,142,46,44,239,105,71,12,250,14,24,104,
118,248,174,45,173,95,255,1,230,184,22,233,169,75,149,77,197,90,102,11,207,215,245,177,133,231,227,39,28,41,181,169,125,244,225,135,146,154,102,235,82,94,152,174,2,229,185,185,242,83,97,142,28,225,238,140,94,63,222,57,166,93,178,227,180,74,140,92,107,
37,59,206,238,166,62,80,5,141,122,42,136,100,169,127,103,171,16,210,66,253,108,98,166,227,76,203,118,156,19,178,246,87,71,2,200,228,24,166,96,21,69,216,1,219,20,214,121,200,143,37,128,132,77,135,240,166,159,228,199,16,64,164,134,2,136,148,163,202,1,36,
236,19,231,173,238,227,239,22,69,248,136,249,124,29,164,0,178,61,214,115,27,101,0,9,15,100,18,237,57,63,0,1,164,40,218,206,114,5,53,23,21,62,206,74,70,57,54,151,53,26,116,128,3,72,65,216,20,45,111,228,99,75,88,253,217,230,10,94,251,155,220,32,178,177,
10,231,187,74,239,107,2,8,8,32,135,102,0,121,200,113,55,25,52,251,124,172,202,177,171,93,233,229,117,245,170,86,155,231,68,63,242,161,233,235,233,98,245,11,142,80,157,248,12,57,170,101,66,113,189,199,168,166,9,146,236,119,196,23,23,47,91,159,123,174,
86,117,218,159,83,199,227,248,19,69,245,167,229,21,189,76,112,69,163,32,234,124,125,191,40,40,77,210,236,227,122,232,161,135,170,116,223,111,188,241,134,60,246,216,99,242,252,243,207,203,15,63,252,80,45,143,103,230,172,89,230,216,86,244,78,182,155,46,
170,192,180,180,151,221,157,61,217,231,156,171,195,68,90,25,163,28,102,100,195,103,195,69,40,222,202,117,5,213,207,26,39,216,81,141,188,176,0,210,92,253,108,92,134,227,76,82,97,227,232,204,253,213,161,34,244,237,145,78,213,136,50,128,132,79,163,8,196,
24,64,246,249,93,140,83,176,182,150,154,63,158,95,157,35,32,85,60,247,82,73,33,118,160,42,161,32,218,243,117,0,166,96,109,142,240,178,17,157,219,104,2,72,85,167,173,149,51,5,107,114,5,83,176,246,169,83,240,234,30,98,232,16,71,50,5,171,32,218,250,157,
8,166,47,201,65,156,130,149,31,54,26,181,37,236,58,219,188,122,159,8,94,251,177,6,144,109,213,49,253,145,0,2,2,200,161,21,64,150,233,247,114,110,138,95,62,156,29,176,163,31,58,124,172,105,42,114,219,25,118,37,171,168,71,62,220,169,87,247,44,22,185,114,
164,221,164,112,121,208,76,185,58,171,95,138,89,122,215,11,34,39,78,157,42,181,177,157,112,226,52,115,124,115,187,36,217,78,251,178,242,167,95,61,56,217,214,177,180,110,127,184,236,222,189,59,182,229,128,239,184,195,236,102,30,12,6,205,109,37,38,38,74,
143,30,61,100,221,186,117,178,125,251,246,42,61,150,7,31,124,208,220,102,219,44,159,252,176,88,29,243,121,121,114,235,248,116,239,57,248,115,203,68,91,203,209,38,113,95,173,84,152,208,69,234,122,132,35,197,13,34,122,47,143,204,56,2,72,88,199,34,166,101,
120,43,233,160,111,45,245,169,106,81,5,157,167,130,50,62,117,158,28,182,122,82,101,5,228,149,221,126,196,69,232,81,78,147,218,24,54,181,167,160,58,2,72,216,109,22,68,83,76,92,213,243,21,195,8,78,233,162,228,205,17,20,161,23,69,80,176,30,209,185,173,206,
0,18,201,57,119,95,207,219,74,21,58,111,169,96,26,209,118,247,120,138,34,152,18,84,84,201,115,186,57,172,64,124,114,25,161,201,123,191,77,14,187,92,183,24,30,255,240,10,106,115,162,121,190,43,10,32,229,190,231,203,24,177,42,40,21,74,164,156,21,209,194,
111,211,123,110,98,57,223,145,190,246,242,203,155,174,70,0,1,1,228,208,10,32,29,149,157,250,189,124,207,209,25,182,238,163,48,219,186,110,162,173,251,136,37,124,232,145,15,125,221,171,199,184,27,17,102,218,162,115,93,131,160,58,191,47,157,150,45,233,
113,186,243,235,151,167,158,124,178,86,6,16,61,2,161,207,75,86,188,35,159,206,13,218,165,121,203,10,32,27,114,165,200,157,74,86,180,98,101,212,247,163,119,120,159,54,109,218,62,251,118,148,214,177,99,71,51,42,19,107,219,177,99,135,52,110,209,206,220,
214,115,39,103,137,156,159,39,207,159,146,45,73,241,182,14,164,87,138,115,216,168,52,199,25,146,186,175,222,201,142,51,59,71,133,19,245,85,101,11,19,58,76,208,32,128,148,55,119,122,123,164,27,17,86,178,250,212,182,176,255,204,183,149,19,106,2,97,29,229,
205,229,76,229,146,176,206,220,214,104,247,40,41,181,172,110,81,117,4,144,176,78,207,182,240,115,86,213,0,82,234,124,108,143,97,21,172,152,207,87,12,1,36,124,89,90,239,19,235,138,150,225,221,92,106,217,223,130,74,62,213,175,240,220,86,87,0,137,244,156,
187,151,219,28,246,90,218,84,193,82,175,129,82,75,185,198,52,5,43,44,132,108,171,100,137,220,252,82,239,225,109,81,4,215,240,199,239,29,111,126,21,159,239,138,30,111,133,239,249,240,17,140,82,27,24,110,171,104,52,40,236,181,191,181,172,235,71,113,190,
35,121,237,77,174,174,81,39,250,125,4,16,252,178,59,83,15,234,247,241,212,246,137,118,55,114,189,19,184,222,235,227,252,174,238,82,187,179,163,15,31,102,153,222,249,118,103,116,29,62,244,232,199,210,176,149,175,206,206,149,7,38,101,154,130,237,110,189,
250,85,123,237,199,219,111,191,45,175,189,246,154,252,231,63,255,145,111,191,253,182,74,183,213,183,255,64,211,105,255,189,94,17,172,172,21,177,150,219,229,119,199,54,179,133,244,119,221,125,79,212,247,49,99,198,140,10,195,135,167,113,227,198,230,63,
185,88,219,145,199,78,54,183,163,55,128,212,123,130,236,92,18,148,78,185,197,187,162,247,202,116,167,92,105,122,58,150,254,183,14,29,227,85,104,232,168,2,72,60,1,228,144,110,101,173,24,68,59,32,231,189,70,231,209,211,104,7,248,245,188,41,218,15,28,8,
32,32,128,212,189,0,114,188,126,15,231,165,249,229,157,89,118,39,114,89,148,41,82,84,79,228,119,211,236,254,29,209,4,15,189,210,149,153,174,165,190,223,168,2,204,194,116,119,223,143,176,145,3,93,204,189,46,36,211,219,197,153,142,239,175,46,253,117,181,
5,143,255,251,191,255,147,65,131,6,73,40,20,146,204,204,76,201,202,202,146,150,45,91,74,81,81,145,188,243,206,59,49,221,230,69,23,93,100,167,137,181,117,87,144,90,94,42,128,168,159,125,183,40,40,245,18,237,104,142,222,75,36,154,118,231,157,119,70,20,
62,60,167,159,126,122,204,231,103,237,186,245,238,148,178,68,91,88,175,130,211,196,214,238,206,243,62,103,114,170,207,214,125,4,84,144,168,151,96,191,119,8,32,135,122,224,40,61,189,100,27,103,134,0,66,163,85,225,245,188,165,186,150,230,165,223,71,0,193,
47,179,51,165,186,137,206,171,250,61,124,201,144,52,91,227,176,212,45,60,191,164,143,45,60,143,118,181,171,59,230,217,205,9,207,57,92,100,126,178,29,253,88,90,106,218,210,234,144,236,88,24,148,134,73,118,185,218,167,159,126,186,202,193,227,179,207,62,
147,81,163,70,85,216,113,111,212,168,145,220,117,215,93,81,223,182,94,78,215,159,144,44,245,19,28,249,106,126,208,134,144,125,234,63,66,242,86,65,182,237,196,135,14,147,47,190,248,34,170,219,175,236,184,75,211,193,234,205,55,223,140,233,60,221,126,251,
237,118,143,146,70,142,93,104,96,67,174,204,118,247,45,201,240,59,235,116,29,136,46,46,15,169,32,209,60,209,142,132,248,8,32,135,114,71,161,91,36,211,75,104,4,16,26,237,96,52,250,125,4,16,252,50,59,83,51,244,251,183,77,48,78,126,116,119,37,55,155,13,
234,221,202,111,62,201,78,161,138,106,218,213,28,59,245,234,188,206,118,10,87,97,176,220,130,237,173,211,179,236,148,162,22,109,228,251,239,191,175,82,248,208,215,31,56,112,96,196,29,248,135,31,126,56,170,219,215,171,80,53,111,219,209,92,247,165,83,179,
247,175,3,81,143,231,245,25,54,128,228,183,235,28,85,1,186,14,43,135,29,118,88,84,1,68,187,225,134,27,98,58,87,143,62,250,168,185,126,143,28,199,142,68,157,155,43,151,14,179,27,18,166,251,156,155,250,170,32,209,40,193,93,94,151,0,66,163,209,104,52,2,
8,8,32,168,198,206,84,146,242,162,126,255,254,102,148,87,219,16,180,163,31,23,246,116,107,63,162,216,112,80,143,122,220,173,174,115,233,64,55,124,4,202,95,178,118,125,174,60,56,201,238,73,113,212,49,19,171,60,250,177,122,245,234,168,58,239,45,90,180,
144,175,190,250,42,170,251,24,54,114,140,185,238,230,163,50,212,241,151,31,64,218,117,233,25,213,237,190,250,234,171,146,156,156,28,117,0,57,255,252,243,99,58,87,122,180,73,143,58,117,15,150,4,144,63,29,107,159,11,149,29,158,59,44,222,137,211,83,175,
24,1,161,209,104,52,26,1,4,4,16,84,119,103,106,162,126,239,182,10,196,153,250,5,51,29,71,143,126,232,81,139,27,38,71,95,251,161,3,203,166,35,237,109,152,154,143,156,10,3,200,245,163,237,167,238,167,156,122,90,149,194,135,30,65,240,150,172,141,198,101,
151,93,22,213,253,20,204,154,109,174,119,209,224,52,91,168,95,78,0,105,213,177,107,84,5,245,31,125,244,145,169,87,137,246,248,175,188,242,202,152,206,215,227,143,63,110,174,95,28,64,206,201,149,167,167,101,121,183,187,87,133,141,103,147,252,206,197,42,
92,12,104,148,224,36,102,17,64,104,52,26,141,70,0,1,1,4,213,212,153,250,179,126,239,158,59,48,181,164,67,173,119,60,63,187,189,93,245,42,154,209,15,29,86,110,85,65,98,101,35,145,69,105,251,215,124,236,55,5,43,36,231,13,180,159,250,47,152,63,191,74,1,
68,215,116,68,219,121,215,134,14,29,26,101,241,246,6,115,189,229,122,19,191,50,2,200,127,220,0,146,219,164,101,212,83,202,122,247,238,29,213,177,199,199,199,199,188,28,239,125,247,221,103,110,163,95,61,199,174,118,182,54,87,190,94,152,35,51,59,39,73,
189,52,127,233,251,122,58,201,231,20,170,0,210,120,162,10,18,71,164,56,78,28,1,132,70,163,209,104,4,16,16,64,16,67,103,170,149,178,35,53,193,39,239,22,100,155,14,180,233,72,47,74,23,185,106,148,157,74,21,241,232,199,44,91,43,114,97,15,59,245,170,162,
224,17,22,64,86,247,177,1,100,249,242,21,85,10,32,103,157,117,86,76,1,164,83,167,78,81,221,207,121,27,47,48,215,43,236,145,108,87,143,10,223,144,112,77,72,62,157,27,16,213,7,23,95,66,186,188,245,214,91,81,221,246,165,151,94,26,213,177,143,30,61,58,230,
243,117,197,229,87,152,219,152,170,87,244,210,59,187,235,81,144,229,33,243,24,62,56,51,32,247,78,202,144,89,93,146,164,73,230,62,97,228,211,80,156,115,129,10,27,45,245,178,188,57,4,16,26,141,70,163,17,64,64,0,65,148,157,169,69,250,125,171,151,95,53,5,
213,43,66,118,218,212,202,134,34,183,158,42,114,231,220,232,166,94,93,119,172,187,209,96,86,196,1,100,67,63,27,64,150,22,22,86,41,128,172,93,187,54,166,0,162,55,245,139,102,170,212,217,231,158,107,174,183,172,103,25,1,100,133,93,154,183,71,40,182,34,
119,189,65,96,164,69,244,9,9,9,242,236,179,207,198,124,190,78,59,253,12,115,59,23,14,74,221,119,36,71,135,144,181,118,85,44,253,154,248,108,126,64,110,153,144,46,195,154,38,132,223,255,231,169,62,103,99,32,206,169,31,32,128,208,104,52,26,141,0,2,2,8,
34,236,76,233,157,29,254,170,223,183,255,167,55,163,243,150,222,93,168,58,164,27,187,216,61,60,34,157,126,165,119,58,223,60,91,221,70,91,123,253,72,194,135,91,3,114,249,176,84,211,169,61,163,160,160,74,1,228,182,219,110,139,41,128,12,24,48,32,170,251,
89,180,100,169,185,222,134,126,169,251,79,193,90,102,31,211,140,78,182,179,126,121,12,245,25,31,124,240,129,116,233,210,165,194,99,78,74,74,146,155,111,190,185,74,231,171,107,111,27,116,182,28,159,105,131,84,89,207,207,50,119,183,122,253,218,88,25,50,
151,61,186,85,98,241,113,168,23,208,59,201,62,231,52,93,168,30,34,128,208,104,52,26,141,0,2,2,8,1,164,146,102,166,95,5,147,125,242,246,204,128,29,1,89,18,176,59,149,95,51,54,186,233,87,197,133,231,153,21,175,122,85,70,0,185,243,232,116,211,153,157,114,
226,212,42,117,168,63,252,240,67,73,77,77,141,58,128,156,119,222,121,81,221,207,196,73,199,155,235,233,226,249,50,59,238,234,103,215,140,178,143,105,228,152,113,49,61,150,79,63,253,84,22,47,94,108,118,59,15,63,214,140,140,12,25,59,118,172,60,245,212,
83,85,58,87,175,188,242,138,196,39,167,73,122,188,35,31,157,25,216,127,63,147,178,184,123,133,200,170,28,185,111,82,166,244,111,84,50,34,162,66,200,29,25,126,39,95,175,150,213,63,205,238,31,66,0,161,209,104,52,26,1,4,4,16,2,72,233,54,85,191,103,7,55,
73,112,231,255,231,216,0,177,162,129,221,249,252,142,185,145,47,187,171,139,213,117,209,250,130,180,200,195,135,219,89,255,199,148,76,187,26,83,239,126,85,94,134,119,206,156,57,81,133,143,6,13,26,152,17,135,72,219,222,189,123,229,136,30,125,204,117,31,
159,154,105,167,42,149,126,76,171,67,242,222,44,91,7,146,144,154,41,111,188,241,70,204,143,231,147,79,62,145,251,239,191,95,174,189,246,90,185,245,214,91,229,245,215,95,151,234,104,23,94,112,129,187,163,123,130,13,31,203,162,120,206,244,107,69,5,145,
159,87,132,228,87,195,211,36,43,201,231,157,207,143,26,196,59,71,247,82,15,188,1,1,132,70,163,209,104,4,16,16,64,8,32,101,180,43,244,123,118,173,158,74,228,21,159,47,76,23,57,167,163,157,78,165,139,202,35,26,253,152,111,151,235,213,163,39,145,214,126,
20,23,109,231,202,135,170,179,158,164,59,235,201,105,49,239,234,237,53,189,11,122,231,206,157,35,14,32,119,220,113,71,84,183,255,254,251,239,75,90,176,158,164,168,235,126,168,71,14,86,151,17,64,150,219,16,50,177,101,156,185,143,53,107,214,74,109,106,
122,115,196,14,157,236,57,186,67,239,101,178,33,55,186,231,44,44,104,233,61,99,94,154,145,45,131,154,196,123,231,116,79,134,223,89,212,68,133,142,0,1,132,70,163,209,104,4,16,16,64,8,32,165,218,131,250,61,171,167,211,236,179,252,238,37,253,220,157,207,
35,8,32,186,70,68,47,189,123,81,47,187,113,225,210,96,116,157,88,93,180,189,42,71,134,29,102,3,193,237,183,223,94,229,14,182,14,9,125,250,244,169,48,120,232,169,90,87,93,117,85,212,183,125,235,173,183,152,235,247,175,239,200,94,239,248,203,122,92,234,
124,62,114,188,29,217,201,173,127,88,212,155,29,214,100,219,188,121,179,57,174,38,105,142,124,191,56,88,118,136,138,114,20,235,231,21,57,82,212,43,165,248,252,166,249,157,75,117,97,122,128,0,66,163,209,104,52,2,8,8,32,4,16,183,5,148,247,226,253,142,188,
120,138,183,252,110,208,78,193,186,118,188,10,32,11,34,27,253,208,211,180,126,63,67,117,98,155,218,165,123,163,237,188,234,169,63,103,229,202,217,253,109,231,117,218,244,83,170,165,147,253,205,55,223,200,57,231,156,99,70,67,244,94,25,94,199,184,97,195,
134,114,220,113,199,201,243,207,63,31,211,237,158,124,234,105,230,118,206,29,224,238,1,178,172,130,96,181,50,36,61,235,219,81,144,245,27,54,212,138,240,177,115,231,78,105,223,233,8,115,76,191,30,154,90,241,99,136,106,36,203,214,135,92,61,50,93,252,62,
119,143,18,199,185,92,47,211,155,79,0,161,209,104,52,26,1,4,4,16,2,136,106,189,148,189,45,3,113,242,237,2,247,83,240,37,89,34,69,245,108,253,71,164,187,159,235,145,146,235,38,218,93,211,163,41,62,47,213,121,125,249,180,108,241,233,145,137,172,28,249,
232,195,15,171,173,195,173,151,181,125,230,153,103,228,238,187,239,150,191,254,245,175,166,80,61,230,233,93,159,126,42,105,217,57,166,115,253,210,201,97,123,166,148,165,80,143,12,132,100,203,113,118,20,36,41,53,93,94,124,241,197,131,30,64,214,184,75,
21,31,30,138,147,29,133,238,174,247,75,171,33,128,232,199,187,194,134,201,63,28,153,81,28,66,146,124,206,175,91,168,0,50,129,0,66,163,209,104,52,2,8,8,32,135,124,0,57,74,191,95,71,229,39,218,34,100,179,255,71,134,250,190,137,221,201,60,210,2,116,29,64,
46,29,104,167,110,197,218,121,45,178,75,189,142,104,106,71,11,46,184,240,66,169,141,237,146,95,253,202,28,223,224,70,113,246,19,255,229,149,60,46,179,159,70,174,20,116,178,203,214,118,233,218,205,140,64,28,172,246,248,227,143,75,92,188,93,185,234,161,
201,153,118,105,221,194,156,234,9,32,94,8,209,231,228,156,92,249,189,10,33,62,119,212,41,43,206,153,123,148,10,26,39,168,0,50,49,115,127,4,16,26,141,70,163,17,64,64,0,57,52,2,200,137,250,253,122,198,17,201,102,51,64,19,2,244,242,187,122,42,213,173,167,
71,30,64,244,94,33,122,231,243,133,233,85,235,188,110,200,149,63,30,147,97,58,172,141,154,52,145,175,182,111,175,85,225,227,203,47,191,84,199,213,204,28,223,157,71,71,81,184,173,130,202,119,75,114,164,109,208,134,171,233,39,159,114,80,142,95,23,247,55,
110,210,212,28,195,226,238,238,6,138,203,171,49,124,148,46,194,63,59,79,174,30,145,86,60,245,45,39,222,25,126,68,178,227,244,82,186,41,221,195,16,64,104,52,26,141,70,0,1,1,228,208,8,32,11,77,103,180,135,219,25,53,1,68,133,136,181,249,34,191,63,35,178,
0,162,151,223,213,171,101,157,115,120,213,70,64,220,61,38,244,178,174,61,235,217,142,122,209,242,21,181,42,128,20,21,45,55,199,213,171,158,95,126,42,170,160,248,188,172,81,129,179,114,229,153,147,178,36,35,193,118,198,231,205,155,119,64,143,253,221,119,
223,149,118,237,218,153,251,30,209,44,65,118,185,35,78,213,58,250,177,223,116,44,91,19,178,176,91,178,23,66,222,109,18,239,100,31,145,228,56,173,18,28,167,117,24,2,8,141,70,163,209,8,32,32,128,28,26,1,228,108,211,209,239,149,18,22,64,50,84,0,105,110,
139,202,35,10,32,238,46,233,235,91,197,86,128,190,223,106,74,33,121,252,68,91,51,225,248,226,229,201,39,158,168,21,225,227,185,231,158,19,159,223,78,93,122,226,196,204,146,29,227,163,41,180,87,157,241,251,38,103,22,143,8,20,20,20,152,61,69,106,186,189,
240,194,11,210,174,125,123,187,207,138,10,119,95,44,12,218,189,75,106,42,124,132,135,144,53,33,249,73,61,246,110,245,227,189,162,244,77,221,82,28,231,220,122,142,179,42,183,4,1,132,70,163,209,104,4,16,16,64,14,141,0,114,169,126,191,110,208,123,128,172,
119,87,66,50,83,176,154,137,220,22,105,0,153,173,194,138,250,186,174,101,245,4,144,101,118,73,215,5,238,167,230,173,90,183,62,232,203,215,110,223,190,93,218,180,181,29,248,185,93,221,149,175,150,135,162,239,140,23,217,218,8,61,125,43,213,45,208,30,48,
96,160,188,246,218,107,53,118,236,122,73,227,236,156,92,115,95,189,15,139,151,15,231,5,76,200,171,209,224,81,198,232,207,179,211,179,36,37,222,110,86,120,120,178,51,252,182,198,142,115,213,97,142,115,181,139,0,66,163,209,104,52,2,8,8,32,135,70,0,185,
90,191,95,55,14,12,11,32,102,23,244,195,68,110,57,217,214,118,84,22,64,54,159,233,6,144,22,213,19,64,220,154,137,31,212,215,238,238,242,181,227,198,141,147,61,123,246,28,180,0,50,110,252,120,59,122,160,142,71,215,114,152,213,194,10,171,16,176,84,135,
252,31,39,101,73,203,108,191,185,221,140,172,128,108,220,184,81,126,248,225,135,106,59,230,255,254,247,191,114,242,201,167,20,143,182,76,107,159,36,59,10,109,184,171,241,145,143,114,70,127,150,246,180,203,44,39,250,156,167,78,203,118,156,235,26,56,206,
197,245,44,2,8,141,70,163,209,8,32,32,128,28,106,1,164,120,47,8,119,31,144,77,19,68,238,94,24,65,0,153,35,114,235,169,34,171,26,219,209,147,234,234,180,170,142,242,27,5,217,210,48,221,118,210,39,77,154,108,118,239,62,144,109,215,174,93,50,125,250,201,
230,254,235,165,250,228,173,153,129,216,119,12,47,61,18,162,66,200,7,115,131,114,66,187,196,226,144,112,248,17,93,229,186,235,174,147,47,190,248,34,230,99,126,229,149,87,100,201,146,66,73,202,8,218,37,112,149,203,134,167,217,208,180,38,247,192,6,143,
112,235,114,229,139,249,65,9,165,218,81,144,150,137,206,216,99,50,28,103,68,154,69,0,161,209,104,52,26,1,4,4,144,67,35,128,92,166,223,175,103,245,47,181,25,157,46,38,63,191,139,29,1,185,189,146,157,208,245,94,33,191,155,170,58,213,121,54,184,84,103,167,
85,29,211,147,97,133,219,19,39,78,52,211,161,14,68,251,238,187,239,228,232,99,142,49,247,155,153,232,200,99,211,98,168,251,136,160,62,66,187,121,66,134,116,115,11,239,181,198,205,242,101,238,220,121,114,231,157,119,154,226,113,189,143,73,89,77,215,143,
124,253,245,215,242,194,11,255,146,171,175,190,90,38,30,59,73,252,201,233,197,183,51,69,133,155,23,79,203,182,207,237,138,208,129,31,249,40,99,228,103,93,191,226,157,210,31,76,247,59,78,134,162,191,18,64,104,52,26,141,70,0,1,1,228,208,8,32,231,233,247,
235,114,93,132,30,30,64,244,102,132,122,131,186,235,39,137,220,179,88,5,141,10,66,200,221,234,247,87,141,178,163,31,177,110,66,88,73,167,245,201,105,89,210,48,213,118,170,219,181,239,32,79,63,253,116,141,134,143,39,159,124,82,58,117,238,108,238,47,47,
217,145,103,166,171,243,113,118,53,237,22,94,58,132,172,180,43,69,237,82,255,190,113,108,186,244,111,224,20,7,8,205,159,30,146,22,237,59,203,192,161,35,204,14,241,179,102,207,145,51,102,158,41,199,30,55,69,186,245,238,47,245,155,183,81,151,43,25,69,137,
87,166,182,75,144,39,212,57,51,163,30,235,220,197,5,14,102,248,8,11,92,31,158,25,144,236,100,51,10,178,167,65,130,211,179,67,178,227,180,77,170,157,1,228,227,143,63,14,40,91,20,209,95,249,175,157,70,163,209,34,111,238,223,206,34,2,8,8,32,4,144,210,173,
208,41,189,12,175,215,97,212,245,28,107,154,139,220,114,186,13,33,101,141,132,120,59,165,87,103,253,71,57,35,33,175,158,145,45,189,27,216,149,148,124,241,137,178,106,245,234,106,31,13,209,251,124,172,223,176,65,28,191,189,159,62,234,254,94,156,145,109,
71,62,150,213,112,7,93,7,145,179,108,113,251,223,167,100,202,134,254,169,50,160,161,95,2,254,125,3,73,89,154,164,56,50,185,77,162,92,61,50,205,76,91,211,27,31,22,7,143,165,181,136,62,135,107,67,50,163,115,146,173,125,241,59,23,119,73,113,156,195,15,226,
62,32,234,63,199,173,202,230,114,2,136,14,31,219,148,225,74,55,186,19,213,214,41,217,26,109,167,68,93,126,178,178,157,179,87,59,158,15,26,141,0,2,2,8,170,18,64,78,210,239,215,57,93,146,203,238,176,234,169,88,122,83,194,235,143,181,211,177,238,90,168,
190,46,176,238,94,100,235,63,46,232,174,194,71,90,205,119,94,85,64,250,174,48,71,22,245,76,46,233,120,55,111,41,23,95,124,177,254,3,87,165,224,241,209,71,31,201,101,151,93,38,13,155,230,23,223,246,156,174,201,230,254,170,92,243,17,109,7,125,165,59,106,
225,6,194,207,231,5,229,249,147,179,228,158,163,51,228,154,17,105,114,241,224,20,249,213,208,84,185,113,116,186,60,116,92,166,252,87,5,179,239,22,5,237,104,135,190,206,218,80,205,109,46,88,29,84,200,210,199,109,70,119,28,231,229,188,120,39,41,20,127,
80,3,200,38,101,99,57,1,68,191,182,54,210,141,56,248,157,18,125,121,125,61,206,94,221,232,36,210,120,109,17,64,64,0,57,180,3,200,177,250,253,122,116,171,68,219,129,45,107,99,61,61,178,161,167,99,157,219,201,78,181,186,241,120,91,243,113,237,120,213,153,
108,107,235,62,170,123,234,85,121,83,120,220,78,246,253,170,3,59,160,81,124,113,88,104,208,168,169,204,56,227,12,121,240,193,7,229,195,15,63,140,56,116,220,127,255,253,50,115,214,153,82,191,97,227,226,219,234,115,88,156,60,160,110,223,132,128,213,7,185,
110,98,185,187,89,224,90,55,92,108,40,69,255,76,215,145,232,208,178,172,156,64,179,218,253,125,109,9,32,234,120,183,207,15,74,227,76,179,184,192,222,122,241,78,231,182,137,7,53,128,152,255,32,75,191,103,220,159,135,27,30,225,39,201,91,74,253,108,91,89,
211,183,220,81,21,111,122,215,118,55,8,5,34,233,120,71,219,25,119,143,97,115,233,235,123,247,231,78,53,219,175,163,160,71,125,74,29,99,65,5,225,96,171,251,117,123,121,151,85,63,203,47,227,188,78,142,224,248,55,149,186,206,182,50,46,163,31,195,70,247,
190,197,61,158,225,81,118,150,242,221,235,121,215,207,175,172,67,85,209,115,225,93,182,212,57,44,170,224,28,234,231,169,192,253,42,238,245,242,203,185,124,192,61,47,222,249,222,92,214,235,39,236,56,10,74,191,62,99,125,62,202,120,124,251,156,179,176,231,
161,220,215,140,123,253,136,46,23,197,241,108,41,253,218,112,127,182,181,26,206,95,81,172,239,191,50,30,175,184,207,113,126,5,239,55,41,239,152,74,189,223,182,85,116,217,154,122,254,220,203,108,11,123,44,147,43,59,119,238,223,60,169,224,61,16,243,235,
161,172,191,85,244,255,8,32,168,221,1,100,136,238,116,31,81,47,94,118,23,186,157,221,178,58,141,186,38,68,143,134,44,206,176,157,218,101,110,193,185,254,153,14,39,75,67,7,174,142,96,185,29,149,248,185,40,71,110,63,42,67,70,52,241,237,51,29,41,62,35,36,
189,250,15,146,51,102,205,150,162,21,171,100,221,250,245,114,238,185,231,169,175,27,212,191,87,170,159,207,145,158,125,7,74,98,86,222,62,215,27,210,208,145,187,142,206,144,61,69,238,168,199,242,90,80,55,81,86,160,240,84,118,89,29,90,84,248,120,171,32,
32,31,207,86,1,177,168,150,132,144,229,246,216,38,182,182,117,43,57,113,206,140,150,181,51,128,120,255,89,110,114,191,15,68,240,159,160,215,177,207,15,251,79,113,191,78,93,216,109,111,118,191,159,236,254,167,185,181,134,2,200,166,240,233,75,97,255,81,
23,148,58,158,110,101,28,231,70,247,235,166,178,46,83,234,113,111,117,59,186,91,194,207,67,57,143,61,154,243,154,31,118,255,195,203,57,134,45,94,199,163,212,241,14,143,50,168,109,113,175,95,84,213,206,104,88,71,178,244,57,44,168,224,28,110,115,95,15,
147,221,239,183,85,212,217,46,117,217,173,149,28,199,230,10,94,139,17,63,31,101,220,110,65,216,107,120,123,216,57,220,92,193,107,102,147,123,204,221,220,235,70,21,126,42,152,166,87,252,156,151,23,172,99,60,127,85,10,32,101,188,62,139,202,121,111,108,
44,245,55,97,115,37,175,149,130,176,127,111,138,33,128,196,250,252,109,116,175,19,126,185,252,10,66,78,190,247,65,75,5,127,163,98,126,61,148,247,183,138,62,32,1,4,181,55,128,52,87,190,214,69,193,111,171,142,170,233,180,86,26,2,178,85,32,201,182,203,245,
30,204,130,230,21,57,110,205,68,142,60,125,82,150,172,234,147,34,61,234,249,36,197,169,188,102,194,113,151,167,237,85,223,39,69,189,83,228,159,234,250,222,210,184,7,125,181,168,106,217,115,195,142,50,44,238,145,34,25,137,62,233,166,2,230,143,139,221,
115,86,43,142,47,87,206,233,159,106,158,135,56,199,185,57,217,231,248,106,91,0,137,101,250,64,88,135,103,99,248,39,122,229,116,70,182,86,212,121,170,230,0,226,221,118,126,216,49,110,245,58,55,222,167,153,17,62,182,162,8,130,87,126,37,159,116,86,235,20,
172,176,78,244,228,202,70,164,34,232,144,109,138,102,74,73,4,1,100,75,25,199,180,181,130,115,216,173,140,199,53,188,156,199,59,188,178,203,150,119,28,213,49,77,166,244,249,10,235,248,85,54,178,150,95,250,249,114,223,19,85,94,232,33,188,211,238,118,172,
165,140,145,134,88,206,95,81,21,222,127,221,98,9,88,21,252,253,40,42,253,184,74,127,200,80,147,207,95,148,127,27,188,0,94,238,123,177,186,95,15,225,199,67,31,144,0,130,218,27,64,84,255,207,121,86,119,4,31,63,49,203,78,233,169,205,157,107,111,4,100,189,
59,253,200,43,182,222,224,126,175,126,247,206,172,128,220,59,49,67,254,111,116,170,156,55,32,73,150,116,79,148,185,71,196,203,226,110,9,178,113,64,178,220,168,126,126,239,177,25,178,77,239,233,177,220,238,79,33,235,106,217,52,165,42,142,14,221,55,41,
67,154,101,249,139,195,214,153,93,146,101,151,14,88,181,101,20,68,157,243,135,79,200,20,159,221,13,254,201,166,9,78,74,93,8,32,238,117,54,123,157,121,247,19,189,77,229,116,146,54,70,210,17,172,166,0,18,40,227,147,206,2,175,195,226,118,14,54,85,50,189,
74,42,11,32,145,118,106,107,32,128,148,249,59,183,227,35,209,222,135,219,249,233,86,77,1,164,40,146,99,170,224,49,68,244,154,168,232,188,86,86,203,84,197,0,82,20,237,235,32,44,24,116,115,59,159,249,229,117,182,99,156,230,36,97,43,216,109,169,202,185,
174,166,0,18,241,229,195,166,202,121,83,181,36,194,247,91,44,163,50,49,61,127,97,161,97,115,216,52,172,138,206,221,246,138,70,35,171,235,245,80,214,223,42,250,128,4,16,212,238,61,13,254,160,59,169,183,77,200,56,176,5,215,49,78,223,249,122,126,142,220,
52,46,93,110,27,159,46,255,57,61,219,142,92,108,112,67,196,138,80,73,49,182,87,39,225,5,140,117,161,125,107,39,244,229,138,114,126,217,161,163,244,212,38,245,184,126,51,186,100,31,144,129,141,18,100,203,148,76,251,88,139,167,148,5,109,205,142,30,201,
50,181,59,7,97,36,75,29,143,94,173,43,59,217,132,164,215,187,36,59,13,235,80,0,25,30,214,217,47,111,250,130,28,200,0,18,246,137,226,102,247,83,206,205,97,159,58,14,47,239,211,217,176,57,242,147,43,26,213,168,43,1,36,236,249,243,58,128,221,106,40,128,
108,63,8,1,164,168,22,5,144,162,50,106,79,164,58,22,25,8,127,157,86,240,186,174,149,1,36,236,253,86,224,118,198,55,214,198,0,18,54,157,202,155,66,154,95,201,185,219,90,201,20,183,42,191,30,202,251,91,69,31,144,0,130,218,29,64,206,213,157,213,185,93,147,
109,65,243,178,90,220,201,86,225,65,7,165,240,169,84,61,26,196,203,202,62,169,242,180,222,171,195,221,83,67,47,101,91,60,221,167,40,204,178,58,20,56,202,216,47,229,186,49,37,225,99,105,175,20,249,105,121,78,241,210,190,178,36,96,23,20,48,203,37,171,208,
81,228,134,77,93,215,163,107,121,204,180,186,3,116,188,234,121,250,100,78,80,154,100,154,205,23,191,203,137,119,122,215,149,0,18,54,242,177,189,130,255,112,163,158,130,85,106,186,197,230,24,58,27,69,97,29,129,201,225,83,148,194,167,93,148,55,82,19,201,
20,172,131,24,64,170,101,10,86,132,83,74,182,151,53,109,165,130,199,185,57,146,99,42,231,121,142,102,10,214,228,72,167,16,213,162,17,144,124,167,6,90,88,189,197,246,74,94,47,145,158,191,136,159,243,104,94,159,149,141,140,214,226,0,178,207,229,34,152,
130,85,20,54,13,109,99,5,35,32,249,85,120,206,203,252,91,69,31,144,0,130,218,29,64,142,212,29,214,190,13,227,229,103,243,73,122,45,158,138,180,74,23,85,103,203,252,238,201,50,184,73,130,164,197,239,91,128,126,92,219,36,121,108,106,86,201,8,71,97,29,13,
28,251,45,81,28,146,103,79,206,18,191,187,103,200,249,3,82,109,240,240,22,21,88,156,101,131,199,134,214,34,151,13,17,185,110,162,200,255,157,32,114,195,36,145,43,71,136,156,115,184,93,104,64,135,145,3,113,188,43,66,242,227,226,160,169,77,113,236,126,
32,71,215,177,0,82,84,73,7,188,116,17,122,129,87,252,89,193,252,241,45,238,229,54,197,242,105,113,216,237,132,207,241,222,88,81,125,128,247,169,101,169,66,211,234,8,32,225,197,222,129,8,143,191,32,108,100,41,146,34,244,205,49,20,161,111,12,123,62,202,
251,244,124,179,183,154,85,248,39,183,149,20,249,22,85,86,24,95,170,144,127,120,4,133,209,91,75,21,81,111,175,224,121,172,44,128,68,253,124,84,177,3,27,8,123,109,77,14,123,188,221,202,57,223,91,163,124,173,79,142,96,218,89,52,231,47,226,231,188,146,251,
219,167,8,189,244,185,46,245,126,243,254,38,212,198,0,50,185,212,235,122,75,36,163,71,97,127,111,134,199,250,122,168,228,53,188,223,223,42,250,128,4,16,212,238,0,210,66,249,38,39,197,47,219,102,102,219,142,123,109,174,115,88,225,238,147,177,50,71,222,
61,51,32,247,77,202,148,83,58,38,73,106,66,73,24,153,219,45,89,190,88,24,172,253,83,202,170,41,148,125,175,206,75,167,92,51,154,32,51,143,72,178,225,195,60,143,33,187,67,189,190,220,21,195,68,54,159,105,247,111,185,107,190,221,68,82,187,123,161,200,29,
243,68,126,115,164,58,167,135,217,209,144,154,62,102,119,68,106,68,243,4,115,204,169,62,231,244,58,22,64,188,229,36,3,17,76,245,241,230,72,111,172,96,25,208,130,176,121,212,94,16,145,24,63,37,220,82,70,40,41,168,224,24,183,133,21,146,110,43,231,19,204,
104,3,200,228,176,199,211,45,194,99,15,223,149,126,115,57,191,223,84,234,60,69,19,62,2,17,46,151,27,8,123,222,182,68,48,5,107,115,216,237,110,171,100,41,99,9,91,202,88,42,89,26,54,16,214,49,46,119,25,231,8,3,72,212,207,71,53,214,16,108,175,104,41,215,
242,234,165,34,12,171,249,149,60,223,145,158,191,136,159,243,8,239,175,204,101,158,75,189,223,182,148,94,46,187,150,213,128,108,10,127,175,184,143,103,99,4,247,177,181,156,101,180,35,122,61,84,242,247,116,191,191,85,244,1,9,32,168,221,1,68,23,162,63,
162,59,130,183,78,72,255,229,116,218,139,236,158,18,94,157,199,182,89,1,89,212,189,100,147,194,30,245,227,229,77,29,168,214,215,241,16,162,30,255,13,99,210,204,99,110,158,29,39,95,235,77,9,215,132,74,150,79,214,117,30,155,38,136,220,179,196,110,28,249,
135,130,253,119,180,191,93,5,147,63,170,223,255,118,138,200,242,6,53,63,18,82,100,131,228,49,173,236,82,188,201,62,103,238,47,236,61,19,201,116,11,54,118,59,196,91,52,175,3,54,90,172,112,212,46,63,202,235,108,143,118,73,90,90,221,105,244,251,8,32,248,
101,117,166,214,58,238,238,223,166,67,255,75,43,206,94,238,22,153,159,149,43,207,76,207,146,142,33,59,26,208,56,195,47,175,232,66,117,253,152,234,226,116,172,34,59,2,50,176,177,29,73,184,106,68,154,29,253,48,163,69,65,59,154,113,233,64,21,46,22,171,240,
49,123,255,224,177,79,8,153,101,67,200,181,71,218,122,144,154,172,9,113,3,200,164,54,54,128,248,28,103,94,93,8,32,97,159,188,110,166,27,64,35,128,84,249,252,21,68,51,250,17,54,53,104,75,180,155,242,209,8,32,32,128,224,224,116,166,244,84,5,105,25,136,
147,29,139,131,181,103,26,150,238,4,235,78,244,194,212,18,186,150,161,172,206,113,97,78,241,94,30,159,46,8,202,136,166,182,83,222,54,232,151,175,244,168,192,218,58,56,18,178,38,36,239,206,10,72,130,223,39,126,159,35,31,170,239,77,16,51,59,216,103,170,
112,210,68,228,214,211,68,238,152,91,113,248,8,31,9,217,172,46,123,118,251,154,157,138,229,6,144,137,110,0,81,230,212,145,0,210,173,170,155,169,209,8,32,180,152,207,119,126,117,236,168,78,35,128,128,0,130,3,215,153,74,80,94,215,239,221,71,79,200,60,200,
211,150,66,37,59,175,23,213,19,57,247,112,145,75,250,138,92,172,92,210,71,117,142,219,217,203,20,175,220,20,42,115,90,210,183,42,144,116,117,139,156,39,171,142,174,41,200,94,30,170,91,43,95,169,199,121,251,145,118,85,176,190,13,19,236,70,131,222,126,
38,250,252,92,172,206,215,93,11,34,11,31,158,187,213,229,175,28,105,167,97,21,6,107,180,6,100,116,126,130,23,64,78,175,43,83,176,104,52,26,141,70,0,1,1,132,39,35,242,118,165,126,239,206,239,158,82,242,41,250,193,232,84,235,142,175,14,32,23,245,18,249,
221,84,59,117,200,43,154,214,157,105,253,41,253,141,199,139,156,223,197,22,88,235,203,150,19,66,94,159,145,45,105,110,113,250,61,71,103,212,173,169,88,110,0,185,116,104,154,91,124,158,108,131,163,55,125,78,143,20,93,62,44,250,0,162,207,243,13,147,75,
246,11,169,161,61,75,126,82,207,67,239,134,241,94,0,57,150,0,66,163,209,104,52,2,8,8,32,135,94,0,25,168,223,187,45,178,227,228,219,5,193,131,176,51,184,187,98,211,50,213,137,222,116,148,93,157,73,119,158,117,0,209,245,9,198,153,110,32,113,127,119,197,
112,59,10,178,56,179,236,16,114,118,158,108,28,148,106,58,185,61,27,196,203,222,229,238,42,90,117,40,128,92,60,196,6,144,165,61,82,246,173,223,41,14,32,243,163,15,32,58,224,233,93,211,107,42,128,168,215,214,103,115,3,210,50,96,54,34,252,81,25,64,0,161,
209,104,52,26,1,4,4,144,67,47,128,196,43,255,208,239,223,59,244,104,193,89,7,120,26,150,222,40,111,177,10,19,215,140,183,197,208,58,112,148,181,98,147,161,126,126,187,10,34,247,44,22,185,108,104,201,230,122,165,111,115,109,72,190,94,24,148,220,84,211,
209,149,199,167,100,213,157,165,121,221,0,242,171,240,17,144,117,161,125,3,136,62,55,209,142,128,232,192,114,253,177,53,59,2,178,58,100,150,80,174,159,102,158,151,247,148,214,4,16,26,141,70,163,17,64,64,0,57,244,2,136,110,23,233,247,239,152,252,68,187,
148,235,129,28,45,88,152,34,114,97,15,59,242,81,217,138,77,30,93,92,173,63,177,47,175,104,218,221,33,124,94,87,187,60,239,188,110,201,182,24,189,40,84,103,150,224,253,253,4,187,251,249,160,38,9,118,212,106,69,78,73,13,200,198,110,234,252,204,83,231,106,
86,132,1,164,192,158,255,95,15,86,215,207,168,209,226,121,189,58,89,122,162,153,30,167,55,25,11,16,64,106,87,163,40,186,218,206,99,248,30,46,195,121,125,212,218,199,195,210,217,4,16,16,64,112,16,58,83,199,40,119,40,95,235,247,111,162,223,145,151,79,201,
50,59,108,31,144,142,180,222,169,123,121,174,173,249,136,186,104,122,145,200,166,10,150,142,93,151,43,127,57,206,22,106,183,203,137,147,157,139,115,14,194,244,178,26,178,54,36,175,169,142,188,126,108,186,51,255,229,188,64,201,30,32,122,90,154,46,226,
191,105,154,123,78,11,42,63,151,122,159,16,61,242,180,182,133,59,170,84,83,59,183,231,202,157,71,103,120,245,31,127,83,18,9,32,116,48,163,56,182,173,149,108,172,183,181,54,44,133,28,182,39,204,198,104,119,25,175,237,231,153,0,18,209,185,219,28,203,235,
183,138,247,187,169,172,205,11,9,32,32,128,160,182,117,166,38,40,79,186,29,65,179,111,134,94,138,87,127,95,224,77,233,89,118,0,58,210,139,244,254,21,109,221,14,240,153,209,5,16,125,29,189,212,236,234,166,110,45,200,254,59,133,127,54,47,40,89,73,126,73,
140,115,228,189,218,190,219,123,52,86,228,200,222,162,28,57,220,221,247,228,111,122,5,179,240,41,102,122,217,226,243,143,176,53,51,122,183,243,138,166,180,233,81,39,189,89,225,175,7,217,231,163,166,166,95,45,179,1,100,81,143,20,47,128,92,82,219,254,211,
160,213,250,0,82,217,206,222,155,162,221,61,155,81,130,232,206,51,1,36,162,32,176,241,64,143,182,48,114,73,255,147,0,194,73,168,237,157,169,60,229,119,225,193,99,77,191,20,249,223,130,128,60,61,53,203,252,44,35,201,39,219,10,178,205,167,236,53,190,114,
148,158,46,116,126,87,183,147,60,43,186,0,162,3,139,174,7,57,231,112,117,59,101,124,106,191,34,36,187,22,5,229,240,92,219,73,127,116,74,230,193,91,229,171,38,58,243,235,114,101,126,55,219,153,159,173,55,146,84,255,46,14,141,58,68,232,194,254,75,251,219,
81,16,93,223,97,106,107,188,115,236,126,175,167,105,233,122,154,171,199,217,90,28,179,178,88,77,133,166,144,25,129,26,218,180,120,9,222,163,8,32,181,162,3,166,55,113,219,230,118,142,204,167,183,229,117,100,244,167,171,110,7,107,187,107,115,121,159,184,
122,157,45,247,54,43,220,36,206,29,41,216,94,222,229,220,125,30,164,148,201,81,118,156,135,135,29,203,182,242,246,109,9,59,230,162,176,199,89,16,109,71,48,204,150,50,126,191,197,125,204,101,78,207,170,234,177,186,251,210,108,141,118,115,190,72,206,115,
216,227,43,136,228,252,196,122,44,238,117,183,133,143,38,132,221,119,32,236,245,88,230,115,30,233,253,186,191,223,228,189,238,221,199,83,84,193,249,217,82,209,229,202,58,158,40,94,191,85,57,87,4,16,250,125,4,16,212,218,206,148,254,143,78,23,254,74,182,
10,25,231,12,74,147,207,245,70,125,186,232,92,83,129,99,92,11,187,65,220,92,93,51,113,214,1,168,153,208,193,225,194,158,182,158,35,154,240,81,188,121,222,28,145,243,142,40,59,128,20,217,157,210,199,186,123,78,220,60,62,253,32,239,115,82,205,84,224,120,
98,90,166,121,108,129,100,191,124,238,77,195,42,244,246,84,201,182,35,26,231,117,22,249,221,52,91,55,99,194,136,27,72,180,91,78,181,123,134,232,69,0,22,149,179,162,88,53,30,239,219,5,1,111,121,228,111,149,38,4,144,90,51,85,104,147,251,125,113,231,185,
156,203,111,241,58,196,97,193,101,107,5,29,59,47,132,120,29,228,205,21,4,16,239,54,203,188,92,25,199,26,136,52,128,148,113,221,141,21,116,2,139,194,194,88,65,88,135,51,63,138,78,188,55,29,70,223,87,183,114,110,127,187,123,28,249,213,112,172,91,220,203,
110,10,187,237,130,138,206,103,20,175,137,64,172,231,199,61,23,197,143,211,189,236,214,40,71,19,182,151,122,253,137,23,120,194,142,181,91,172,247,27,246,58,221,88,234,28,22,148,51,133,106,171,123,185,201,229,132,199,72,95,131,129,106,62,87,4,16,250,125,
4,16,212,202,206,148,254,180,121,151,126,143,234,79,160,159,59,45,75,228,236,220,146,186,1,51,63,63,36,143,159,104,71,65,82,85,39,113,155,158,178,84,211,35,6,122,4,68,7,136,88,71,64,254,112,166,221,160,112,81,57,1,100,85,72,78,108,159,100,30,211,45,58,
128,156,147,87,167,2,136,14,84,253,26,217,128,117,222,160,84,145,115,243,74,86,195,90,234,46,167,171,207,241,242,6,118,74,150,158,102,165,55,27,188,108,176,200,5,61,212,249,105,92,178,177,227,178,80,141,23,206,223,56,38,221,27,253,120,168,54,254,167,
113,8,6,144,253,58,57,229,141,128,132,117,160,134,87,244,179,210,159,44,151,215,153,172,224,152,54,150,119,185,8,166,96,149,215,249,43,235,113,110,45,61,58,81,170,131,157,95,234,211,235,162,234,232,12,134,221,254,240,40,158,147,72,143,53,16,235,121,143,
118,10,86,36,231,199,189,239,109,165,62,225,143,184,40,223,237,228,139,123,31,129,240,81,186,176,215,202,182,170,220,111,57,163,84,91,203,234,252,187,151,221,92,133,115,87,209,239,170,122,174,8,32,244,251,8,32,168,117,157,169,33,202,78,253,254,44,232,
156,44,123,86,218,21,162,204,116,157,194,82,29,246,181,185,114,108,27,59,10,114,108,235,36,217,187,34,39,172,67,91,67,53,32,235,90,218,233,65,145,174,128,85,188,18,150,10,45,55,77,23,89,161,58,215,75,50,203,14,32,171,67,114,106,71,27,64,102,233,218,22,
93,3,162,31,251,138,208,47,115,99,194,66,183,144,94,61,134,31,213,247,23,14,73,149,64,178,221,112,49,41,206,49,1,82,206,117,247,4,41,126,124,65,91,35,163,131,134,166,195,154,247,189,254,121,77,237,122,94,106,3,66,81,175,187,225,205,138,167,95,205,35,
128,212,154,249,239,27,35,233,200,84,240,115,137,100,42,74,37,157,242,110,110,240,217,94,201,8,76,172,1,100,123,25,143,115,99,164,143,179,38,2,72,5,215,173,214,99,141,165,99,26,109,13,72,5,231,125,155,219,177,206,15,19,241,185,12,11,29,225,163,57,5,94,
160,114,131,194,166,170,220,111,57,83,166,42,60,223,238,237,199,18,50,42,250,93,85,207,85,65,89,97,140,0,2,2,8,14,86,103,42,71,121,87,191,55,103,168,142,184,169,235,88,93,78,231,187,208,126,170,174,151,73,141,243,233,250,144,56,217,177,56,88,179,75,242,
234,154,3,253,41,253,13,147,236,50,176,209,4,16,93,187,112,213,104,91,235,80,86,225,116,145,173,59,152,216,58,209,235,244,202,232,102,137,242,168,222,19,196,29,61,168,209,112,85,19,157,248,13,118,212,234,174,137,25,210,179,126,241,110,226,210,42,219,
214,185,212,75,243,203,83,39,169,199,119,78,110,244,207,219,178,146,58,141,154,24,173,121,110,122,150,36,197,155,176,244,165,210,144,0,66,0,41,53,245,100,147,27,68,54,214,64,0,145,95,80,0,145,58,20,64,164,28,209,156,203,45,110,56,245,234,52,242,195,166,
183,85,84,11,20,209,253,86,16,64,182,87,48,69,109,115,233,145,166,106,8,32,85,58,87,222,148,72,2,8,8,32,168,45,157,41,189,210,144,244,60,44,94,246,172,200,169,188,176,124,153,157,182,244,196,212,76,121,122,90,150,237,244,46,61,0,133,232,122,26,213,29,
115,220,189,43,10,42,223,179,194,91,178,87,175,128,85,222,178,177,170,35,253,221,194,160,217,225,93,159,131,80,138,191,184,195,174,71,121,30,209,163,5,171,220,17,145,85,181,184,56,221,59,70,245,120,238,157,156,41,195,75,138,184,165,115,94,188,60,124,
124,166,108,87,151,59,222,29,185,74,142,243,201,245,99,210,247,125,108,203,42,9,29,58,144,169,112,243,173,58,95,219,231,7,171,55,152,185,193,233,212,142,201,222,113,95,83,91,255,211,56,4,3,200,214,50,166,159,68,51,5,107,114,37,83,91,34,9,32,251,252,188,
134,2,72,212,211,154,14,98,0,169,214,99,173,13,35,32,85,124,141,122,133,246,197,197,248,222,249,40,111,217,217,104,238,183,172,105,85,229,157,239,8,67,97,149,70,64,28,26,1,4,4,144,58,16,64,244,28,221,175,227,253,142,60,166,87,128,210,157,209,72,167,29,
233,218,143,53,7,168,83,174,167,0,233,34,242,75,7,216,81,13,93,44,93,81,0,209,33,69,23,173,235,205,11,245,114,179,229,238,57,17,146,135,79,176,69,218,173,2,113,242,70,65,182,172,237,151,34,41,246,83,120,99,82,155,36,121,64,117,234,247,46,119,59,235,107,
66,181,99,84,164,200,110,218,167,143,105,119,81,72,254,56,49,179,120,129,0,27,166,124,114,238,192,84,217,177,216,157,78,183,62,87,118,169,48,113,114,135,164,226,203,28,221,42,81,30,159,154,101,3,198,134,220,226,209,19,67,7,209,245,238,207,244,238,228,
179,178,229,138,225,169,210,54,39,206,140,124,189,113,70,118,245,61,255,234,126,222,42,40,222,124,240,103,165,43,1,164,214,4,144,130,82,5,184,149,21,161,111,45,85,132,190,189,188,142,90,20,1,164,160,140,105,54,82,65,39,109,75,53,20,161,111,170,32,56,
29,236,0,82,173,199,26,99,0,41,247,60,71,25,64,138,74,189,190,134,71,179,162,88,216,244,60,41,181,250,213,198,178,106,55,98,185,223,82,139,37,84,118,190,195,111,175,162,58,145,162,88,207,107,172,231,170,188,227,33,128,128,0,130,131,209,153,154,231,117,
68,205,180,171,218,60,221,72,79,197,90,156,33,114,73,95,91,140,174,131,136,14,26,186,46,100,243,153,246,171,14,29,119,171,159,255,254,12,91,184,94,209,158,21,69,246,83,247,105,110,135,188,176,103,138,200,249,121,166,240,254,205,89,1,179,202,87,106,88,
16,233,82,47,94,46,24,156,38,111,232,194,251,21,110,24,89,231,78,99,90,118,0,167,88,173,45,25,181,216,166,142,243,66,117,76,157,242,74,166,90,229,164,248,101,85,223,20,249,120,94,112,223,145,155,194,156,226,112,241,155,209,233,18,76,46,121,108,221,234,
199,203,242,94,41,114,215,49,25,178,245,228,44,249,247,233,89,242,236,244,44,185,123,98,134,172,234,147,98,70,84,226,252,78,241,229,59,229,198,203,71,179,3,213,179,103,202,50,27,102,103,118,46,30,253,184,167,54,255,167,113,40,182,176,21,170,188,213,148,
138,42,89,134,119,115,216,242,171,155,42,91,134,55,194,78,249,166,176,2,227,141,21,124,178,61,57,236,88,187,69,217,249,43,189,220,240,240,170,126,194,95,19,1,164,186,143,53,198,0,82,238,121,142,246,252,120,181,9,97,171,115,109,137,225,53,186,207,245,
194,66,73,65,37,225,186,210,251,13,11,191,225,203,30,23,148,243,218,223,18,190,188,114,57,43,127,85,246,26,172,232,245,27,243,185,114,143,103,139,115,8,55,250,125,4,16,212,158,206,212,31,245,123,242,170,225,105,251,110,82,87,43,185,203,198,234,233,88,
235,91,139,92,51,78,228,150,83,236,180,44,61,34,162,3,200,205,211,237,10,78,107,155,139,44,240,194,71,168,220,79,221,223,83,29,120,183,230,64,94,58,57,187,100,9,94,183,254,227,173,130,128,172,84,157,249,38,153,37,83,179,244,134,133,186,102,228,198,177,
233,230,83,123,211,9,215,231,110,189,91,59,179,220,13,36,158,88,59,229,203,74,86,233,50,163,77,27,236,50,200,159,204,9,200,31,142,76,151,41,237,147,204,74,100,222,113,53,207,242,203,186,254,41,242,129,14,6,103,229,150,61,58,81,152,83,28,158,222,84,65,
106,137,10,93,57,97,211,206,42,51,170,121,130,220,160,30,247,174,37,57,213,83,247,83,104,67,224,11,42,244,164,196,23,223,79,111,254,211,0,0,0,4,144,186,27,64,254,237,247,57,230,211,238,95,204,254,23,222,116,44,93,215,177,226,48,91,27,114,78,71,213,145,
109,171,58,255,245,75,86,113,90,26,172,120,36,65,133,140,233,238,232,135,174,247,48,35,11,69,165,87,252,178,157,245,175,23,229,200,77,170,227,125,84,171,68,209,231,43,188,83,62,176,113,188,172,86,33,229,254,201,153,242,31,61,45,105,153,59,210,176,206,
157,194,164,173,117,3,193,170,114,172,118,167,60,185,117,22,246,58,182,216,251,67,21,40,30,61,49,83,46,26,146,38,99,91,36,122,123,100,20,27,223,50,81,110,26,151,46,223,46,46,41,64,175,116,36,107,153,251,216,212,229,63,153,19,148,205,71,165,203,236,46,
201,50,160,81,188,116,173,23,47,71,228,197,155,81,17,253,239,89,234,231,250,246,95,61,61,44,108,85,87,61,140,251,248,7,53,46,174,89,185,138,79,173,0,0,0,1,164,142,7,144,172,100,159,188,61,51,96,59,201,21,117,88,107,93,24,9,95,58,54,181,100,201,216,72,
70,23,206,201,149,123,39,102,20,119,226,95,48,1,44,84,121,145,183,250,170,59,226,151,14,79,147,145,205,19,37,222,191,111,24,72,84,255,238,167,58,237,51,85,167,125,67,191,20,211,113,215,133,250,111,23,100,203,246,5,65,249,113,73,80,118,169,0,181,211,229,
125,255,221,162,160,153,210,244,202,169,217,242,231,99,51,228,226,33,169,178,184,103,138,76,80,225,66,215,114,148,30,137,232,211,48,94,206,31,156,42,47,156,162,66,193,170,156,170,21,201,175,46,25,93,209,231,102,79,97,142,236,86,246,120,231,202,173,51,
217,47,160,85,135,179,115,229,186,81,105,222,227,250,84,169,79,0,1,0,0,4,144,58,30,64,116,225,239,155,103,4,236,167,239,165,167,199,44,179,251,126,120,159,198,255,34,247,197,40,253,120,206,205,149,127,169,142,126,182,91,3,113,129,222,156,111,67,110,100,
163,6,203,67,37,29,114,213,225,127,123,86,64,238,60,58,67,230,118,77,150,1,141,227,37,37,193,87,238,244,165,204,36,159,52,76,247,75,211,44,191,52,83,154,187,95,245,244,174,220,84,191,196,251,203,159,250,212,49,55,78,78,233,152,36,215,143,78,151,87,103,
100,219,227,240,66,65,117,45,129,92,20,182,196,174,103,69,13,213,5,185,83,175,94,86,207,67,122,201,57,59,137,121,187,0,0,128,0,82,247,3,200,203,186,127,247,212,180,82,83,176,188,90,1,61,63,255,148,108,41,234,149,34,79,79,205,170,221,75,209,86,70,119,
168,207,201,149,231,79,203,86,157,127,187,236,238,184,252,68,251,152,162,221,215,162,112,223,41,90,102,186,149,250,247,182,153,1,249,227,196,12,185,102,84,154,44,236,158,34,163,155,39,72,139,108,191,100,169,240,161,55,1,44,47,96,232,61,85,244,212,42,
189,71,71,159,195,18,228,228,195,147,205,148,171,219,142,204,144,231,79,206,146,93,139,221,145,136,13,238,125,173,200,249,229,134,193,66,123,222,118,169,239,245,116,47,247,28,220,78,225,32,0,0,32,128,28,26,1,68,175,56,36,87,140,72,43,9,32,133,238,168,
199,234,144,92,58,52,77,18,220,142,243,10,21,66,138,119,71,255,37,117,120,117,192,56,219,142,88,220,58,33,189,248,19,247,161,77,18,228,187,37,17,236,123,18,205,232,193,234,80,201,6,134,235,237,57,220,185,36,40,159,207,11,200,39,115,3,242,238,153,1,121,
125,70,182,249,228,255,69,21,236,254,173,194,208,155,5,217,242,225,156,128,124,170,46,243,205,130,160,29,221,40,163,22,196,212,173,44,251,5,143,64,121,175,45,183,230,229,196,118,197,203,1,191,165,100,16,64,0,0,0,1,228,208,8,32,139,117,255,238,84,189,
3,250,26,119,245,38,213,241,253,126,113,80,166,181,47,217,47,66,127,255,174,94,93,105,69,45,29,1,89,17,42,25,201,88,21,42,25,45,80,193,99,79,81,72,30,153,146,41,227,195,246,200,152,212,38,81,118,120,65,171,176,134,247,234,88,233,6,147,213,222,254,26,
238,242,189,158,181,238,207,87,187,199,94,23,130,70,121,225,99,165,125,94,86,245,78,241,158,139,159,90,37,58,253,218,38,58,78,179,132,242,241,94,5,0,0,4,144,186,19,64,122,40,123,27,103,250,229,75,189,111,196,5,121,242,234,204,236,226,125,37,130,73,62,
185,105,124,122,241,167,249,181,110,218,79,161,93,209,106,199,162,160,124,60,39,32,95,168,199,160,87,117,122,233,212,108,51,218,161,247,195,216,119,143,12,159,92,57,50,173,100,213,169,194,58,216,209,175,229,225,227,252,129,169,222,243,241,99,40,222,153,
180,54,215,113,174,172,239,56,23,215,43,31,239,85,0,0,64,0,169,59,1,36,65,121,66,119,8,117,49,245,219,179,3,210,204,221,243,162,77,48,78,182,170,142,188,174,155,48,211,130,10,99,236,120,122,181,18,53,81,63,162,130,132,158,210,116,120,110,156,4,146,125,
114,88,186,45,232,46,93,99,161,127,190,178,79,138,169,209,48,211,200,86,18,62,14,248,180,43,21,62,46,28,156,90,178,98,152,207,89,212,72,189,250,142,203,116,156,211,179,29,103,122,86,249,120,175,2,0,0,2,72,221,9,32,186,233,29,93,37,63,59,78,242,82,109,
125,132,46,158,254,114,81,176,234,53,31,234,186,186,6,226,177,19,51,205,6,122,21,46,245,187,50,134,144,178,38,215,212,81,52,202,176,161,67,215,171,36,248,29,9,169,16,50,168,73,130,44,237,153,34,127,60,38,67,190,90,24,44,217,35,99,25,161,224,192,22,156,
219,243,94,212,43,121,159,80,232,115,156,77,250,197,87,47,222,113,154,196,57,78,163,10,240,94,5,0,0,4,144,186,21,64,114,148,79,188,142,225,73,29,146,100,215,50,187,83,120,68,163,4,21,237,248,189,46,36,127,159,146,101,110,183,91,189,120,249,98,190,27,
106,74,215,73,172,179,193,224,135,197,65,251,239,104,70,39,150,231,200,231,243,130,242,218,140,108,179,51,249,155,103,100,155,105,88,230,152,188,29,196,87,135,8,3,7,218,50,187,63,201,143,234,235,201,29,74,234,137,230,182,73,144,122,238,222,38,126,199,
153,221,34,193,113,218,43,109,42,192,123,21,0,0,16,64,234,86,0,209,173,80,119,8,235,167,249,101,199,98,119,99,187,194,200,167,214,152,145,133,50,87,160,202,149,15,102,7,205,158,23,250,246,59,132,226,228,85,21,18,204,170,84,69,37,215,215,59,125,235,128,
210,52,211,47,111,23,4,162,31,9,89,21,86,200,237,125,173,169,61,44,16,89,189,199,217,118,201,99,189,163,186,126,238,147,227,28,185,125,96,138,200,156,76,185,161,79,241,104,200,142,128,223,105,149,159,64,17,58,0,0,32,128,28,106,1,36,83,121,61,193,239,
147,75,135,165,217,0,82,84,201,134,126,42,56,236,86,255,190,100,104,154,220,125,76,70,217,171,55,185,123,62,252,103,102,182,180,207,177,123,111,232,233,82,247,31,151,41,114,94,158,233,168,174,237,95,82,23,80,63,221,47,239,168,192,242,139,222,111,228,
80,31,245,88,103,11,252,55,141,73,147,172,68,59,210,209,34,195,39,127,27,169,194,199,233,25,178,107,122,186,236,57,41,67,134,214,143,243,166,98,93,22,136,115,156,44,37,187,28,188,87,1,0,0,1,164,238,5,16,221,198,154,58,10,191,99,234,42,202,28,5,241,62,
221,86,191,123,120,74,150,116,206,179,157,200,198,233,62,217,229,109,94,88,86,199,116,125,174,124,177,48,40,227,243,75,150,194,93,213,39,85,142,105,85,242,239,185,221,82,204,101,8,31,191,80,171,237,168,199,171,5,129,125,158,215,99,155,196,203,255,142,
83,161,246,180,12,249,254,196,116,249,250,132,116,19,68,238,27,154,44,126,159,185,204,7,74,58,251,128,0,0,0,2,200,161,23,64,116,187,67,247,247,6,52,74,144,159,151,187,69,225,133,97,225,195,93,190,246,226,97,105,250,147,107,211,193,108,149,237,151,59,
142,201,144,189,43,42,168,5,209,215,213,123,94,168,128,114,214,128,212,125,138,145,243,82,253,242,251,163,50,108,205,201,154,80,245,125,18,95,228,162,232,188,230,119,152,87,193,99,135,250,254,130,33,169,18,72,182,211,237,210,226,125,114,73,247,36,217,
59,93,5,142,83,51,228,235,41,233,198,55,138,156,156,33,95,30,159,38,45,220,197,3,18,125,206,144,20,245,130,74,46,7,239,85,0,0,64,0,169,187,1,36,87,121,87,119,10,23,116,115,119,62,95,17,42,158,74,181,87,125,127,122,167,146,130,226,69,61,82,228,187,66,
183,96,125,89,37,117,1,58,160,156,159,39,155,85,216,136,247,151,172,182,245,241,188,128,173,9,89,94,197,142,176,217,65,60,84,178,131,184,87,11,226,253,108,173,187,2,22,203,239,86,143,53,54,120,236,44,12,202,111,198,166,75,203,64,92,241,235,98,98,147,
120,121,237,168,52,145,25,25,242,211,180,146,224,225,217,57,213,134,16,125,57,125,249,56,199,89,148,164,130,70,98,57,120,175,2,0,0,2,72,221,13,32,186,141,240,58,146,183,31,153,110,195,129,234,108,238,93,25,146,41,237,108,248,208,211,180,110,28,147,94,
210,177,175,116,179,64,61,109,43,36,215,142,74,43,238,164,206,234,156,36,187,139,108,45,73,149,70,58,86,149,172,116,181,173,32,32,247,29,155,41,23,12,78,149,51,187,36,203,204,35,146,101,125,255,20,185,231,152,12,249,72,239,228,94,157,163,44,135,162,162,
146,224,241,245,162,28,185,78,5,143,30,13,74,54,122,236,144,229,151,223,245,75,150,159,221,81,143,111,167,236,31,62,180,239,167,218,223,207,110,147,224,93,119,131,202,25,78,121,120,175,2,0,0,2,72,221,14,32,197,171,98,37,199,59,242,204,180,44,179,25,225,
201,135,219,240,145,153,232,147,199,166,170,159,157,235,142,90,20,86,190,76,174,14,8,215,140,44,9,31,58,20,200,250,80,213,118,87,95,109,131,199,87,11,130,114,157,10,67,35,155,37,236,183,1,97,184,140,4,159,204,82,161,228,173,89,238,102,132,203,25,13,137,
116,153,99,179,151,135,217,192,49,71,94,58,45,91,86,247,77,45,222,123,197,108,90,169,190,191,178,87,146,236,152,106,235,59,118,77,43,59,120,104,186,6,228,199,19,210,68,166,103,200,180,252,226,231,108,25,1,4,0,0,16,64,14,237,0,162,219,111,117,223,175,
117,48,78,102,29,97,151,77,77,137,247,201,22,189,130,213,57,185,145,47,113,123,118,174,220,59,41,179,184,179,186,166,111,138,29,137,88,21,99,248,112,3,205,15,203,114,228,202,17,105,251,76,253,209,179,117,122,31,22,47,167,119,76,146,243,7,165,202,198,
193,169,50,91,133,142,158,97,159,210,235,93,209,239,212,171,118,157,85,133,29,222,235,250,40,199,10,91,235,227,141,112,125,56,55,40,183,78,72,151,9,173,18,36,206,157,62,167,181,207,246,203,175,123,36,201,167,147,109,240,216,171,66,133,14,24,197,225,227,
4,27,56,190,62,94,57,46,77,81,63,59,49,93,246,170,203,238,153,153,45,125,235,23,63,47,71,82,132,14,0,0,8,32,4,144,56,229,79,94,103,211,239,243,201,35,39,168,240,177,49,55,242,90,138,117,185,242,102,65,64,50,220,229,88,245,206,228,166,83,187,50,198,142,
191,238,20,171,219,220,172,2,68,155,96,73,240,232,174,58,178,215,140,76,55,27,17,154,219,214,5,239,27,114,75,234,65,86,229,200,63,167,103,201,145,45,74,86,103,186,118,164,59,133,108,249,33,190,108,174,183,216,192,122,119,148,67,157,227,93,75,114,228,
149,211,179,229,242,225,105,50,190,101,162,164,38,150,132,14,189,42,238,228,166,241,114,215,224,100,217,161,194,132,94,221,74,79,185,250,118,74,233,176,145,102,254,253,237,137,25,242,189,10,38,63,158,145,37,63,205,201,150,189,122,165,51,245,252,232,229,
150,179,108,193,250,118,189,33,58,1,4,0,0,16,64,8,32,186,165,41,247,234,62,96,82,156,79,54,31,229,214,132,120,5,221,149,108,14,184,103,69,142,244,107,104,63,229,30,151,159,80,178,97,96,97,108,187,106,111,95,20,148,211,58,21,111,98,39,109,85,8,185,249,
200,12,179,31,137,57,46,119,165,173,125,142,109,153,187,82,211,57,121,230,254,151,171,16,228,93,255,254,201,153,54,132,44,171,100,71,247,131,29,18,42,82,20,102,185,27,40,244,57,88,233,6,11,111,115,198,181,97,188,194,252,117,118,36,234,135,37,65,121,94,
5,180,219,38,100,200,130,238,201,210,41,47,110,191,41,108,189,66,113,114,118,151,36,121,113,66,170,200,116,27,60,246,76,179,129,227,43,21,54,190,242,2,199,212,116,249,254,228,76,249,97,70,166,252,52,59,32,123,116,224,40,244,142,207,221,28,82,61,87,23,
15,45,94,13,237,129,202,94,132,188,87,1,0,0,1,228,208,9,32,222,72,200,117,94,71,244,164,14,73,178,205,171,163,88,95,70,135,127,169,59,133,231,220,92,249,149,219,201,212,59,156,127,54,207,126,242,29,245,174,218,43,108,248,120,232,132,76,105,158,101,59,
198,137,126,71,54,14,73,149,31,151,185,193,99,101,40,178,240,176,218,238,97,178,186,183,13,33,185,41,126,249,74,133,26,51,170,179,62,108,212,196,27,57,89,239,118,210,215,229,238,219,129,95,19,182,227,122,52,214,148,82,28,8,194,238,103,125,152,178,142,
199,8,89,235,194,142,103,85,201,238,239,187,151,228,200,119,170,227,255,249,220,160,188,83,16,144,87,78,205,150,173,39,107,89,242,196,212,76,121,64,5,175,223,140,78,151,57,93,146,101,80,147,4,105,154,189,127,224,8,38,250,100,244,97,113,114,121,207,36,
121,110,92,170,89,205,74,23,142,139,250,186,195,157,94,165,167,83,125,55,61,83,118,156,154,41,59,103,101,201,158,121,1,217,171,207,231,178,176,48,84,84,42,220,169,99,253,74,29,91,243,172,226,26,146,201,4,16,0,0,64,0,33,128,148,213,22,42,187,188,58,138,
53,253,82,228,191,122,202,83,248,244,157,176,14,243,187,103,6,165,65,154,237,100,222,50,62,221,214,141,20,70,25,62,86,218,219,190,118,116,73,1,187,174,231,120,250,148,44,27,60,162,45,98,119,151,19,214,199,236,173,222,52,177,117,162,186,253,116,185,102,
68,154,220,52,46,93,238,62,58,67,254,162,58,232,143,157,152,37,207,158,148,37,47,171,206,251,235,167,103,155,21,182,222,87,193,235,163,57,1,249,223,220,128,10,84,1,249,114,126,80,190,94,104,125,163,169,206,247,183,46,253,253,55,238,207,245,239,183,47,
80,129,64,93,231,83,117,221,79,148,15,102,7,228,61,117,123,250,118,255,51,35,91,254,173,238,231,5,21,18,116,209,255,99,83,178,228,161,227,50,229,207,199,102,202,230,35,51,228,119,99,211,229,218,145,105,242,235,97,105,114,209,16,45,213,172,244,181,97,
64,170,20,169,48,53,183,107,178,156,220,49,73,38,181,77,146,81,249,137,210,191,81,130,217,36,178,85,32,78,234,171,231,32,57,222,87,97,129,190,22,80,129,99,92,195,56,217,208,57,81,30,26,145,98,106,56,246,234,208,113,178,165,151,206,253,118,106,134,124,
175,194,134,158,78,181,235,204,108,217,163,30,211,207,222,249,47,10,185,129,163,130,48,168,67,201,185,121,178,170,79,241,40,212,51,74,34,1,4,0,0,16,64,8,32,229,181,174,202,93,94,167,53,77,117,90,7,53,78,48,133,222,23,14,78,147,75,134,166,201,210,94,41,
50,161,101,130,52,76,247,23,7,6,51,138,177,58,20,93,80,112,71,24,10,123,148,76,153,154,163,58,218,59,189,165,123,99,157,46,165,111,91,133,161,63,29,155,81,105,167,124,31,62,71,226,227,28,73,73,240,73,122,146,79,50,147,125,18,76,241,75,110,90,137,188,
116,75,127,223,32,195,47,45,178,227,164,89,150,253,119,48,213,47,89,234,58,25,234,186,105,74,162,186,157,248,56,159,185,221,168,142,35,114,63,123,223,231,36,58,210,46,211,47,29,213,177,12,80,193,100,104,253,56,25,223,40,94,10,219,37,200,205,253,146,229,
145,225,41,242,209,196,52,51,186,177,84,253,108,122,139,4,121,241,200,84,145,121,42,96,46,12,200,94,245,117,143,10,91,123,23,135,213,141,148,53,186,81,209,57,215,151,57,55,87,30,156,148,233,237,128,174,13,136,228,69,199,123,21,0,0,16,64,14,221,0,226,
181,163,220,32,178,189,130,14,240,94,253,245,183,227,220,186,145,104,2,130,10,30,63,175,10,201,140,206,37,245,30,87,12,79,179,83,148,214,228,86,125,229,42,117,219,159,207,11,74,227,76,51,245,104,87,162,207,185,61,209,113,174,138,119,156,191,123,171,105,
181,201,240,73,243,52,159,228,169,176,144,165,194,66,138,95,5,144,106,12,8,126,199,222,94,170,186,221,140,56,29,18,124,210,32,217,222,167,190,239,206,1,191,244,203,141,147,193,42,44,100,38,152,235,252,164,46,127,87,154,223,185,36,221,239,92,92,63,222,
89,23,138,51,203,37,207,83,230,198,57,206,156,100,159,115,122,146,207,57,94,253,126,164,186,220,16,245,243,167,28,183,104,124,239,73,233,242,131,158,50,117,188,250,122,66,186,236,210,5,228,167,40,51,50,69,10,148,57,89,242,195,172,108,239,156,216,189,
90,84,176,252,207,204,108,251,252,157,227,142,56,21,197,118,190,245,245,255,126,98,166,132,82,139,167,94,93,18,233,139,141,247,42,0,0,32,128,16,64,188,214,76,153,160,172,83,174,80,174,82,86,42,235,149,31,130,41,62,121,255,204,64,116,155,255,173,8,153,
221,213,143,107,155,84,188,127,199,189,147,50,108,7,120,69,53,45,155,171,110,103,199,162,160,87,108,189,51,47,222,105,125,152,234,221,171,175,93,77,193,117,174,95,118,78,179,27,230,125,50,57,77,222,59,54,77,222,58,38,77,254,123,116,154,188,122,84,170,
188,162,29,153,42,47,151,225,37,229,237,73,105,114,243,0,27,158,122,134,252,230,58,255,57,218,94,71,251,183,250,247,107,138,190,189,109,19,211,228,29,229,35,117,157,79,143,75,147,237,102,229,40,21,18,244,158,26,167,232,205,252,50,164,75,208,116,218,191,
204,138,115,234,53,85,73,169,149,50,54,221,113,6,166,238,91,168,163,2,136,163,119,20,87,1,196,9,168,31,164,250,156,30,234,199,123,116,205,204,117,125,147,69,230,171,48,49,51,75,126,86,100,78,192,140,110,252,188,216,157,70,85,100,131,223,59,103,6,101,
110,247,148,125,70,102,78,104,151,36,127,154,148,41,187,205,174,247,37,27,63,86,24,70,194,54,45,252,121,121,72,46,29,154,38,73,113,197,183,121,147,187,197,7,1,4,0,0,16,64,8,32,213,210,122,235,41,64,195,155,37,148,76,213,137,52,28,172,11,201,217,253,237,
180,171,116,21,62,30,153,146,105,63,129,175,206,141,3,85,0,249,94,5,144,142,185,118,4,68,117,214,187,171,240,225,100,198,153,48,37,71,54,142,51,171,60,233,0,160,55,204,147,147,93,167,68,232,204,76,249,219,72,251,24,38,54,85,183,53,35,195,236,147,81,230,
101,189,219,158,174,151,179,205,144,189,39,101,200,30,197,43,250,254,80,5,147,124,187,233,223,39,217,113,78,179,122,234,56,91,37,56,206,208,52,199,233,149,82,118,0,73,83,1,36,164,126,144,229,55,191,90,110,118,175,87,157,127,189,35,188,174,193,48,35,18,
225,53,27,69,165,150,58,94,159,43,207,157,154,45,5,97,35,80,90,231,188,120,179,9,225,179,211,179,76,33,121,241,94,33,27,202,40,158,87,207,163,94,181,236,110,117,159,67,155,238,179,73,228,53,209,132,15,2,8,0,0,32,128,16,64,34,105,19,117,191,241,180,78,
73,209,141,126,184,43,85,61,56,57,67,166,180,79,146,199,167,102,218,226,246,234,94,30,87,221,199,187,179,2,146,103,139,228,191,82,29,251,230,25,126,211,137,159,101,142,187,85,130,9,7,223,149,179,155,119,101,116,112,184,161,175,237,188,159,220,34,193,
4,12,189,75,120,196,183,225,110,224,167,107,50,94,26,151,42,161,36,83,68,190,77,133,143,122,58,128,232,209,154,209,233,42,229,85,16,64,114,212,15,50,253,197,191,190,64,31,139,174,57,185,97,76,186,61,167,21,45,135,92,148,83,188,34,151,222,199,229,236,
129,169,102,201,227,240,48,146,155,226,147,99,90,39,202,178,158,41,178,81,253,254,186,209,233,242,187,241,233,102,170,220,154,62,169,50,173,67,146,169,127,9,187,206,235,202,137,177,188,152,120,175,2,0,0,2,8,1,164,178,118,156,183,100,175,153,170,19,109,
128,88,233,238,99,177,38,138,235,122,59,120,71,184,59,251,239,143,76,247,58,198,79,15,78,117,18,243,237,90,76,171,245,207,150,119,76,52,35,32,223,196,26,64,212,117,127,213,221,78,33,91,216,206,134,153,239,79,220,63,96,20,111,220,119,124,154,229,237,167,
113,188,123,59,167,103,202,115,71,165,153,145,32,117,91,47,168,128,145,170,3,134,95,233,166,194,199,160,212,136,3,136,110,151,122,97,64,175,156,181,119,165,93,146,184,220,209,41,111,90,150,187,103,136,94,214,247,129,227,50,165,160,115,146,180,14,198,
69,83,239,178,85,209,181,42,185,177,190,152,120,175,2,0,0,2,8,1,100,159,230,115,59,192,30,213,239,213,5,208,210,181,94,188,236,92,18,69,48,216,103,135,238,8,55,6,44,116,111,95,5,157,189,75,221,125,67,42,11,55,42,216,140,104,102,167,5,169,78,250,210,150,
73,166,94,66,183,141,250,103,103,119,169,98,0,57,53,67,86,119,180,187,174,111,232,148,104,54,238,251,214,13,24,58,104,152,61,52,166,216,77,251,190,59,41,195,110,220,167,194,198,206,153,118,121,219,159,230,186,27,248,173,201,145,167,167,101,73,130,223,
116,228,183,132,159,115,157,45,178,253,81,5,16,221,22,121,171,99,13,106,148,96,151,51,174,108,52,36,124,51,71,119,239,145,189,234,223,175,21,4,228,87,67,82,37,57,222,28,219,247,202,157,110,109,199,13,202,121,110,113,124,239,234,120,125,241,94,5,0,0,4,
16,2,200,62,45,221,237,240,234,194,231,160,237,248,102,170,31,255,55,222,239,147,231,79,206,178,117,1,133,53,176,75,184,183,92,239,218,144,172,238,155,34,125,27,38,200,75,186,83,93,209,146,191,122,53,166,19,50,189,79,231,191,235,149,226,52,234,90,50,
146,160,87,102,146,141,221,202,9,32,39,132,141,94,28,95,106,244,194,13,24,250,247,122,202,213,153,173,109,192,185,178,119,178,200,172,76,249,241,244,44,21,50,178,100,103,129,10,24,179,179,139,55,237,219,235,21,129,47,11,219,45,220,179,33,87,254,114,124,
241,177,222,85,217,110,145,17,4,16,221,198,41,239,153,101,148,19,124,114,214,192,84,217,17,190,161,99,36,203,233,174,180,171,90,233,175,29,114,204,104,200,15,74,135,154,26,78,227,189,10,0,0,8,32,4,144,226,79,225,227,149,84,191,237,244,134,127,245,187,
83,126,206,236,146,108,139,146,151,135,170,63,128,168,96,179,107,121,142,217,135,196,155,242,243,232,148,44,59,109,168,156,229,125,127,84,95,187,214,183,155,16,102,198,57,103,245,81,225,99,100,186,157,206,148,228,56,55,235,159,95,223,59,201,140,90,124,
117,92,88,184,56,222,6,15,19,68,244,114,182,211,50,228,251,233,118,244,98,199,41,153,54,92,204,204,146,61,122,117,169,165,65,57,177,141,29,1,185,251,168,12,27,192,138,74,5,12,239,223,203,42,216,75,67,93,111,243,81,197,251,149,220,88,77,1,68,183,28,229,
55,222,57,211,245,29,191,155,144,33,123,220,233,105,166,72,61,194,41,111,163,155,39,122,199,55,129,0,2,0,0,8,32,168,209,0,162,71,62,234,39,56,78,40,222,118,120,61,245,18,76,103,184,179,158,238,163,55,238,251,215,169,217,209,239,132,94,17,61,61,235,236,
60,249,120,94,80,70,186,83,169,244,52,165,255,27,155,94,126,205,201,10,187,36,236,210,30,54,172,168,126,250,27,205,18,156,192,160,52,199,25,166,194,199,4,21,66,212,97,255,69,255,238,190,145,169,34,103,102,203,15,51,178,228,199,130,108,51,106,177,107,
150,50,39,91,118,207,11,200,158,5,129,146,145,139,194,176,105,74,69,238,232,128,10,92,19,219,216,26,144,71,166,100,197,94,72,175,2,136,46,26,119,59,248,151,85,22,6,83,34,15,32,94,59,82,121,209,11,34,122,83,201,59,245,74,89,43,34,88,106,119,185,221,223,
35,108,133,171,163,8,32,0,0,128,0,130,26,11,32,13,85,232,24,159,97,71,15,70,164,237,75,255,76,203,240,155,125,65,164,91,253,120,249,102,137,94,182,181,26,66,200,58,187,76,236,29,170,163,156,151,106,138,179,165,89,166,95,158,152,230,118,244,151,151,83,
195,112,110,174,252,97,66,201,238,231,241,62,103,104,182,219,81,79,80,157,246,56,159,25,68,120,50,57,94,5,166,83,178,237,253,148,158,18,21,190,108,237,178,50,120,35,3,107,67,114,84,75,59,50,96,86,242,90,159,27,243,8,143,222,63,195,61,230,179,42,123,78,
226,221,81,144,40,2,136,110,42,119,57,115,244,148,57,239,220,116,205,139,147,77,163,211,229,51,21,240,204,106,88,27,220,58,145,85,46,29,76,206,207,147,55,206,8,152,221,224,213,117,118,40,45,8,32,0,0,128,0,130,26,11,32,135,39,57,206,220,160,227,76,203,
42,219,233,1,179,23,69,158,186,232,127,116,31,242,152,86,137,178,199,140,14,196,48,21,107,153,187,155,246,217,185,242,233,130,160,204,12,155,114,165,139,201,255,59,43,96,167,14,45,43,99,175,144,101,57,197,187,112,103,218,229,108,181,13,241,62,51,229,
42,188,169,163,118,254,93,63,205,47,239,206,10,70,191,124,112,169,13,248,70,53,183,35,3,79,77,203,170,82,0,217,56,40,213,59,230,194,72,158,23,253,184,50,162,11,32,94,203,117,139,212,223,244,206,109,139,236,56,41,236,153,34,143,157,152,37,255,155,23,144,
111,23,6,205,254,41,95,47,206,145,39,79,202,146,222,135,21,143,126,220,93,147,75,170,241,94,5,0,0,4,16,2,136,9,32,11,85,0,57,53,187,108,51,3,182,19,172,90,71,229,127,186,31,57,161,69,162,124,60,63,96,167,99,173,114,87,94,42,172,108,25,216,92,51,178,241,
221,226,160,92,57,50,77,234,165,218,189,37,84,223,90,214,245,79,181,183,179,46,84,126,112,81,247,245,204,201,217,146,85,18,62,54,153,143,253,125,118,180,32,108,71,188,230,122,179,191,246,161,56,249,106,126,48,178,58,136,114,54,57,212,186,187,117,38,47,
155,209,148,24,3,200,134,125,2,200,236,72,159,155,68,95,76,1,196,107,25,202,105,202,223,189,21,179,180,6,233,126,19,72,90,7,226,164,249,190,251,123,188,164,52,33,128,0,0,0,2,8,106,52,128,52,136,183,133,219,122,55,238,178,244,77,181,83,129,220,54,80,249,
82,247,37,219,7,227,228,247,71,101,200,158,229,110,193,243,122,119,74,79,248,244,158,117,238,30,21,107,67,242,222,156,128,92,62,34,93,218,229,148,236,61,49,184,73,130,60,113,146,59,178,176,186,140,37,100,189,240,114,78,158,60,61,61,75,26,165,23,119,152,
239,119,235,181,245,148,43,35,172,181,211,155,18,14,104,156,32,187,189,229,122,99,9,13,234,122,186,152,59,63,219,30,239,187,51,3,118,199,240,24,71,64,54,244,43,14,32,103,70,243,252,232,21,201,178,227,170,188,198,192,80,229,106,229,95,202,7,238,106,87,
187,149,175,149,215,220,85,195,14,115,106,184,241,94,5,0,0,4,16,2,72,44,173,173,251,169,186,233,80,235,17,2,189,4,236,131,199,101,202,59,179,2,178,125,126,208,120,239,204,128,252,237,132,76,249,245,136,52,153,210,46,73,114,146,139,71,46,228,136,188,56,
185,121,66,70,113,161,119,153,83,174,10,75,10,206,31,56,62,211,219,196,207,155,38,148,84,193,241,117,82,190,213,83,186,204,237,174,8,197,188,203,250,55,139,130,102,196,69,215,147,124,161,71,83,86,199,120,91,235,114,101,81,143,20,239,248,79,143,230,100,
39,251,247,221,39,164,26,22,60,11,185,207,161,62,79,249,186,230,253,64,189,112,120,175,2,0,0,2,8,1,36,214,150,226,214,50,188,17,190,91,182,174,187,104,159,19,39,237,66,113,114,88,134,95,175,80,181,207,110,218,1,213,153,191,126,76,186,153,134,85,188,95,
69,97,249,1,64,143,158,252,102,116,154,164,196,251,194,247,208,72,170,228,216,6,41,123,198,181,72,136,110,87,245,210,214,132,228,227,57,1,241,249,236,227,250,113,73,140,211,185,150,217,130,251,211,58,37,121,143,225,184,104,55,135,212,203,33,251,156,95,
126,227,189,10,0,0,8,32,4,144,170,54,93,103,48,69,209,171,100,61,175,124,174,71,31,92,95,40,47,43,87,42,231,235,21,150,244,148,171,226,29,187,43,153,178,180,119,121,168,120,169,93,215,175,34,60,166,73,250,242,211,15,79,178,33,102,121,236,163,22,255,61,
35,96,238,187,131,10,84,102,85,174,88,194,140,91,204,62,177,77,241,62,27,35,163,222,161,222,103,135,46,8,32,0,0,128,0,130,67,61,128,132,55,189,15,121,43,119,106,143,214,218,13,40,186,233,85,180,190,211,133,225,187,244,72,66,121,29,249,21,182,222,227,
221,217,1,25,221,188,120,101,166,189,202,210,40,142,163,64,95,175,176,87,74,201,18,188,49,214,109,108,157,158,101,142,97,64,227,120,59,250,17,75,152,89,110,235,73,70,52,43,126,60,189,157,67,180,241,94,5,0,0,4,16,2,200,129,106,65,229,163,120,191,99,55,
51,60,59,175,100,229,172,101,110,161,248,217,182,88,253,134,177,233,146,147,92,92,108,254,142,50,60,202,251,90,171,175,123,209,144,52,91,220,190,44,246,0,242,215,201,153,230,56,38,182,78,178,5,232,69,49,174,166,165,174,215,187,129,89,77,107,167,210,129,
0,2,0,0,64,0,33,128,212,124,187,73,247,65,79,210,83,163,244,70,134,231,186,27,227,173,15,201,207,170,131,254,196,212,44,153,210,62,41,124,202,213,3,238,40,74,180,205,108,154,248,91,189,155,250,134,42,4,16,117,221,59,142,180,27,30,158,209,57,57,246,48,
163,194,213,143,75,114,164,67,200,4,144,237,74,51,2,8,0,0,0,1,132,0,82,243,173,139,242,147,217,116,176,73,130,92,62,50,77,174,30,157,38,179,187,36,75,151,188,184,240,224,161,107,71,150,84,225,126,116,161,186,60,56,185,10,59,151,47,179,1,228,150,9,233,
230,152,22,117,79,137,61,128,172,14,201,151,243,131,210,36,211,140,234,188,239,78,71,35,128,0,0,0,16,64,8,32,7,160,157,168,124,230,148,90,29,203,181,77,185,188,26,166,40,61,232,247,57,242,220,244,172,216,55,14,116,3,200,77,227,109,0,89,210,51,197,142,
166,196,184,154,150,94,146,56,148,98,86,242,210,187,201,167,19,64,0,0,0,8,32,4,144,3,215,26,185,69,229,55,185,206,114,87,134,202,174,166,219,223,162,247,237,120,229,180,236,170,5,144,245,185,114,213,200,52,19,64,86,245,73,141,61,128,172,13,201,235,167,
103,75,154,221,199,228,57,167,110,44,104,69,0,1,0,0,4,16,2,8,205,237,220,63,171,59,251,186,211,95,213,0,114,118,127,187,123,249,175,134,84,33,128,168,99,208,97,40,217,238,101,242,200,161,252,228,240,94,5,0,0,4,16,2,72,93,107,1,229,109,189,113,160,222,
68,48,230,157,203,139,236,200,197,194,238,118,31,146,255,243,10,218,99,12,32,47,171,0,146,20,103,2,200,95,8,32,0,0,0,4,16,2,72,221,105,122,133,169,237,237,114,226,98,223,185,220,219,187,67,133,151,233,29,237,170,92,247,28,147,17,123,65,187,10,32,186,
30,69,215,165,232,250,20,2,8,0,0,0,1,132,0,82,119,90,55,229,231,1,141,19,236,166,129,203,115,98,15,32,43,66,50,161,165,221,189,252,145,19,50,237,62,32,49,238,39,242,183,227,51,189,66,251,63,18,64,0,0,0,8,32,4,144,186,211,116,49,187,28,211,58,209,78,191,
138,117,15,144,229,246,186,253,27,218,221,203,159,63,37,171,74,1,228,238,99,50,188,0,114,19,1,4,0,0,128,0,66,0,169,59,109,154,238,231,158,217,37,89,100,93,21,2,200,138,144,252,180,36,71,14,207,141,147,56,191,35,111,22,100,199,86,79,226,46,231,123,227,
152,116,47,128,92,70,0,1,0,0,32,128,16,64,234,78,155,169,251,185,235,250,166,198,94,179,225,6,144,31,23,7,165,77,78,156,164,198,251,228,131,51,3,177,213,147,184,1,228,215,195,210,188,0,114,54,1,4,0,0,128,0,66,0,169,59,109,158,238,231,110,28,84,197,0,
178,50,36,223,47,12,74,203,128,95,210,19,125,242,225,236,24,3,72,145,45,66,223,224,46,231,171,44,38,128,0,0,0,16,64,8,32,117,167,173,211,253,220,75,134,86,61,128,124,49,47,40,122,57,223,220,84,159,124,54,183,10,1,100,109,174,44,234,145,226,5,144,25,4,
16,0,0,0,2,8,1,164,238,180,171,117,63,247,183,227,210,171,22,64,86,135,228,157,153,118,239,142,86,129,56,217,177,40,104,66,73,172,203,249,158,214,41,217,11,32,147,9,32,0,0,0,4,16,2,72,221,105,155,117,63,247,129,201,25,177,239,130,174,173,9,153,205,3,
245,109,245,60,44,94,126,246,86,198,138,161,150,68,155,212,38,209,11,32,99,9,32,0,0,0,4,16,2,72,221,104,126,229,209,164,120,159,217,248,47,230,101,115,151,218,93,208,255,49,53,203,132,134,209,249,9,42,68,84,33,128,20,229,200,152,252,4,47,128,12,39,128,
0,0,0,16,64,8,32,117,163,101,40,175,212,75,243,201,54,189,108,238,154,42,4,144,117,33,185,111,146,221,187,227,132,118,73,182,254,99,121,108,171,105,233,149,176,70,52,35,128,16,64,0,0,0,1,132,0,82,215,90,80,249,111,235,96,156,124,49,47,198,162,241,176,
205,3,111,153,96,247,238,152,221,53,217,142,166,20,197,30,64,134,219,0,242,147,210,143,0,2,0,0,64,0,33,128,212,141,86,95,121,187,125,40,78,190,154,31,172,90,0,217,144,43,87,142,176,123,119,172,234,155,162,2,73,140,1,100,165,221,208,176,175,221,81,253,
91,165,19,1,4,0,0,128,0,66,0,169,27,173,173,242,101,183,250,241,242,195,162,156,216,86,173,10,11,32,103,245,179,123,119,92,58,44,205,174,168,21,203,174,234,42,4,125,189,32,40,135,171,80,164,110,235,19,165,57,1,4,0,0,128,0,66,0,169,27,173,191,178,103,
84,243,4,145,162,80,149,3,200,210,158,118,239,142,255,27,155,110,254,29,107,0,249,120,118,80,154,101,153,0,242,150,146,71,0,1,0,0,32,128,16,64,234,70,59,74,247,113,167,117,240,138,198,99,12,32,203,108,0,89,236,110,30,120,203,248,42,4,144,53,33,121,243,
140,108,179,153,161,186,173,23,149,116,2,8,0,0,0,1,132,0,82,55,218,233,186,143,187,176,123,21,138,198,189,0,178,62,87,230,118,181,155,7,222,54,161,106,1,228,95,167,102,75,90,162,89,1,235,113,37,142,0,2,0,0,64,0,33,128,212,141,182,74,247,113,207,31,152,
26,123,205,134,86,100,151,225,157,210,62,201,4,144,251,142,205,180,1,36,198,229,124,31,155,146,41,126,187,4,239,253,135,250,19,196,123,21,0,0,16,64,8,32,117,169,93,166,251,184,55,142,205,136,125,196,194,11,32,171,67,197,123,119,60,113,98,150,13,52,49,
46,231,251,231,99,51,188,61,64,110,35,128,240,94,5,0,0,4,16,2,72,221,105,155,116,31,87,119,248,99,30,177,240,246,238,80,186,213,139,55,193,225,223,167,101,139,172,139,61,128,252,254,200,226,0,114,45,1,132,247,42,0,0,32,128,16,64,234,78,251,157,238,227,
62,52,57,179,106,1,100,101,72,246,46,203,145,230,118,229,42,121,111,150,10,32,107,99,15,32,183,186,27,26,42,23,19,64,120,175,2,0,0,2,8,1,164,238,180,251,116,31,247,177,41,85,152,50,181,212,78,191,250,118,97,80,178,147,124,146,28,239,147,47,244,166,134,
171,67,49,7,144,155,198,23,7,144,141,4,16,222,171,0,0,128,0,66,0,169,27,205,167,252,51,57,78,79,153,202,138,125,202,148,182,54,36,31,207,9,136,223,231,72,131,52,191,252,184,164,10,187,170,171,0,114,237,168,226,0,114,62,1,132,247,42,0,0,32,128,16,64,234,
70,203,82,222,106,152,225,151,159,117,241,249,5,121,118,20,68,91,23,178,203,242,174,9,217,32,177,202,221,164,112,121,142,85,228,46,189,187,172,100,9,222,255,206,200,54,161,161,67,40,206,254,126,69,40,230,13,13,207,31,148,234,5,144,85,4,16,222,171,0,0,
128,0,66,0,169,27,173,177,242,69,189,52,159,220,48,58,85,238,57,38,93,238,155,152,33,143,30,159,41,207,156,148,37,47,157,154,45,111,20,4,228,131,217,1,51,165,234,187,69,65,249,169,48,167,36,92,232,80,178,218,181,49,79,94,63,61,96,66,195,240,102,9,34,
231,169,48,115,118,110,73,160,49,161,38,44,216,148,182,198,189,29,125,155,234,186,107,250,22,7,144,185,4,16,222,171,0,0,128,0,66,0,169,27,237,8,101,183,219,209,47,87,146,18,244,57,210,56,201,145,118,25,142,244,8,57,50,164,161,35,227,155,251,100,82,75,
191,76,110,229,151,233,29,226,101,116,51,91,128,222,50,16,39,231,14,78,149,75,134,166,202,53,163,210,228,166,113,233,114,247,49,25,242,208,113,25,242,248,212,76,121,86,133,155,173,211,149,147,179,228,197,83,178,228,63,51,178,229,253,153,1,249,108,174,
10,57,11,131,178,91,133,145,89,93,146,189,251,159,70,0,225,189,10,0,0,8,32,4,144,186,209,134,233,254,109,126,139,102,50,127,201,153,50,125,198,73,114,204,113,71,203,240,177,35,165,215,160,1,210,177,123,119,201,111,127,184,212,107,222,82,50,234,53,148,
248,172,144,56,137,122,121,220,68,197,39,149,5,151,104,164,232,144,163,52,73,116,164,147,10,57,161,228,226,223,29,69,0,225,189,10,0,0,8,32,4,144,186,209,198,233,254,237,233,167,159,32,182,125,167,124,169,124,42,63,203,71,178,243,167,119,229,155,239,222,
144,207,191,252,183,124,248,209,243,242,214,219,79,203,43,175,62,42,207,109,125,80,254,241,196,61,242,183,71,239,144,135,31,185,67,182,252,109,179,252,253,31,127,150,229,43,230,153,208,112,68,151,195,229,138,43,55,202,134,179,150,73,225,178,217,50,115,
246,41,50,245,164,73,114,204,177,227,101,204,248,145,50,106,220,8,25,49,118,132,12,30,62,72,122,244,233,41,237,59,119,148,198,45,91,74,78,195,198,146,18,80,33,39,37,83,221,78,156,23,64,6,17,64,120,175,2,0,0,2,8,1,164,110,180,99,116,255,118,230,172,147,
212,233,216,33,123,229,3,217,35,31,154,240,33,242,137,242,63,229,51,229,115,55,152,108,87,190,118,125,163,124,235,250,206,196,151,123,254,116,163,9,13,51,206,56,209,13,52,223,43,95,185,215,253,204,189,205,143,213,237,127,108,190,238,217,251,129,252,184,
243,29,19,114,62,251,226,21,249,240,227,23,228,221,247,182,202,235,255,249,135,116,234,212,94,223,214,207,74,55,2,8,239,85,0,0,64,0,33,128,212,141,86,160,251,183,115,230,158,106,130,196,143,187,222,145,31,118,190,29,53,61,82,162,195,198,37,151,174,55,
1,100,225,194,51,204,191,119,238,126,215,220,230,206,159,222,49,151,217,181,251,189,98,63,237,209,222,151,61,63,127,160,130,207,71,38,144,216,192,179,195,124,117,3,200,119,74,43,2,8,239,85,0,0,64,0,33,128,212,141,182,70,247,111,215,174,93,100,70,49,98,
9,31,218,174,221,239,154,0,179,70,221,142,190,189,179,206,94,106,2,72,44,183,165,71,74,254,247,217,203,210,164,73,35,125,91,255,83,14,35,128,240,94,5,0,0,4,16,2,72,221,104,151,235,254,237,85,87,159,111,2,200,142,31,99,11,32,122,52,67,79,181,210,35,41,
250,246,174,188,234,220,42,4,144,47,228,163,79,254,37,13,27,54,208,183,181,77,201,38,128,240,94,5,0,0,4,16,2,72,221,104,191,215,253,219,205,119,108,114,3,200,182,152,66,195,238,189,239,155,58,145,169,83,39,154,0,114,211,45,151,199,60,162,162,3,136,174,
5,105,216,176,190,190,173,87,149,20,2,8,239,85,0,0,64,0,33,128,212,141,182,37,46,46,78,30,121,244,14,83,88,30,235,20,172,61,63,219,194,245,113,227,134,155,0,242,231,123,127,103,138,212,99,9,52,186,96,253,237,119,159,145,220,220,28,125,91,175,40,201,4,
16,222,171,0,0,128,0,66,0,249,229,183,84,229,95,153,153,25,242,175,23,183,152,142,127,172,1,68,135,15,93,108,222,183,111,119,19,64,254,254,247,187,204,148,172,216,2,200,215,242,242,191,31,145,180,180,52,125,91,47,42,9,4,16,222,171,0,0,128,0,66,0,249,
229,183,128,242,159,96,48,32,175,254,231,49,19,24,116,136,136,101,37,44,189,130,149,94,74,183,83,231,118,38,128,60,245,244,159,69,47,217,27,75,77,137,46,102,127,234,233,123,197,239,247,235,219,122,138,65,42,254,102,0,0,0,2,8,1,164,110,180,60,93,228,221,
168,81,3,249,248,127,47,170,211,177,91,246,221,231,227,219,50,124,83,6,125,217,157,162,247,248,232,216,177,173,9,32,255,126,237,81,247,246,62,149,146,189,63,62,146,189,242,161,236,222,251,129,89,126,55,124,73,94,77,47,211,107,151,243,221,33,127,221,242,
123,111,19,194,7,137,31,252,205,0,0,0,4,16,2,72,221,104,122,127,141,207,211,211,82,101,226,49,99,101,102,193,73,50,119,238,105,82,84,52,71,214,175,95,34,23,92,176,90,46,191,226,28,185,254,250,75,228,150,91,175,148,123,254,120,163,60,248,224,173,242,200,
35,119,200,99,255,184,91,30,123,252,30,249,199,19,127,148,167,255,121,175,188,250,250,99,242,194,191,254,42,205,243,155,136,207,231,147,39,159,250,147,252,248,227,199,178,227,135,109,38,108,216,61,62,62,51,5,230,118,99,194,111,196,110,94,248,157,216,
205,10,191,119,191,215,123,128,72,120,0,185,157,248,193,223,12,0,0,64,0,33,128,212,141,214,71,249,201,237,232,199,204,231,248,36,201,73,148,140,132,116,111,218,148,52,173,215,72,58,183,109,47,61,58,29,33,3,122,245,146,145,67,6,203,209,227,70,203,148,
201,71,203,140,83,78,148,185,179,79,147,165,75,102,203,138,162,121,202,124,89,181,98,190,108,88,191,212,132,158,155,126,119,133,156,81,48,205,187,253,223,16,63,248,155,1,0,0,8,32,4,144,186,209,198,235,190,109,240,136,108,233,118,81,39,233,124,86,7,105,
95,212,70,218,46,104,37,173,103,229,75,139,83,155,73,179,19,155,72,163,163,15,147,6,99,242,36,119,72,142,228,244,15,74,176,95,182,4,250,102,75,118,175,76,201,236,146,33,169,237,82,37,41,63,73,252,121,54,124,248,252,42,56,196,87,45,212,132,185,136,248,
193,223,12,0,0,64,0,33,128,212,141,118,170,238,219,54,155,220,88,142,126,107,140,76,120,101,164,140,123,97,68,137,231,135,203,184,173,195,101,236,115,195,101,204,51,195,100,204,63,135,202,152,167,172,209,198,16,25,245,248,16,25,249,247,193,234,119,195,
164,239,245,221,85,248,240,73,122,179,84,25,240,251,222,50,112,115,31,233,123,99,15,233,117,109,55,233,113,121,23,233,122,65,71,19,114,14,95,213,86,218,47,107,45,237,150,106,173,164,205,162,150,210,122,110,190,228,207,104,42,77,167,53,146,198,83,14,147,
180,54,169,94,0,89,69,252,224,111,6,0,0,32,128,16,64,234,70,91,170,251,182,45,79,109,46,227,254,53,194,4,137,17,127,27,180,191,71,172,145,143,14,222,159,186,206,200,199,6,203,248,151,70,74,159,235,237,18,188,129,35,178,77,96,209,183,57,246,217,97,50,
86,133,24,19,102,84,168,25,175,126,54,254,197,82,94,42,249,126,194,203,35,101,226,219,99,205,49,185,1,100,46,241,131,191,25,0,0,128,0,66,0,169,27,77,143,46,72,187,5,173,76,231,127,196,35,229,4,144,8,232,128,209,245,162,78,38,52,228,245,11,201,232,39,
134,202,168,199,42,184,189,71,246,167,3,141,30,81,209,183,213,248,232,134,94,0,57,137,248,193,223,12,0,0,64,0,33,128,212,141,118,158,238,219,182,47,108,237,6,144,65,177,7,144,127,141,144,206,107,219,155,208,112,216,168,250,102,74,214,200,199,162,15,52,
58,180,232,235,54,28,219,192,11,32,71,19,63,248,155,1,0,0,8,32,4,144,186,209,174,211,125,219,142,171,218,85,57,128,232,235,119,40,108,99,87,192,154,220,216,212,142,148,59,165,43,186,0,50,138,248,193,223,12,0,0,64,0,33,128,212,141,246,39,221,183,213,43,
96,233,218,140,88,195,135,13,32,35,165,221,194,214,38,52,180,56,185,153,169,247,208,83,170,162,14,32,255,24,34,163,159,28,42,13,134,231,121,1,100,4,241,131,191,25,0,0,128,0,66,0,169,27,237,73,95,156,79,250,92,215,93,198,61,95,245,0,162,151,239,53,69,
237,167,187,69,237,177,4,144,199,135,200,168,199,134,72,94,159,28,125,91,123,149,33,196,15,254,102,0,0,0,2,8,1,228,151,223,210,149,255,38,100,38,200,128,63,244,54,171,85,85,45,128,140,144,86,5,249,38,128,180,58,35,63,230,0,50,250,137,33,50,226,225,65,
18,236,156,173,111,235,7,165,7,241,131,191,25,0,0,128,0,66,0,249,229,183,134,202,103,169,135,165,200,208,251,251,203,232,167,135,198,30,64,30,177,1,164,249,148,38,38,128,180,93,208,50,230,154,18,189,191,200,48,117,60,25,173,50,244,109,109,87,218,18,63,
248,155,1,0,0,8,32,4,144,95,126,107,175,252,152,213,54,67,70,62,58,72,70,233,145,135,24,3,136,30,233,208,53,31,141,198,217,194,241,142,171,99,47,106,215,65,104,240,61,253,36,181,145,217,136,240,3,165,17,241,131,191,25,0,0,128,0,66,0,249,229,183,78,202,
158,80,143,128,25,117,136,101,201,220,226,0,162,174,171,119,75,207,27,16,50,1,68,239,120,110,2,72,12,183,53,246,153,161,102,23,245,164,96,146,190,173,215,149,0,241,131,191,25,0,0,128,0,66,0,249,229,183,158,186,95,155,219,59,71,198,60,61,52,166,37,115,
195,87,174,26,243,244,48,9,116,49,117,27,210,243,202,46,166,6,36,166,0,242,236,48,233,123,99,15,137,79,139,215,183,245,172,146,76,252,224,111,6,0,0,32,128,16,64,126,249,109,156,238,215,214,27,148,43,99,158,25,86,165,0,162,119,61,31,169,66,72,70,139,52,
19,64,250,222,208,195,76,201,138,41,128,108,29,46,61,175,234,34,254,120,159,190,173,135,137,30,4,16,0,0,64,0,33,128,212,141,118,154,238,215,54,57,166,161,140,123,126,120,165,245,26,58,160,140,126,124,72,153,83,181,244,20,174,225,91,6,73,74,158,153,54,
37,3,245,170,90,42,72,196,180,163,186,58,150,174,23,118,242,246,0,185,155,232,65,0,1,0,0,4,16,2,72,221,104,43,28,119,207,142,114,235,53,30,118,11,204,85,40,208,155,3,14,123,96,128,25,233,24,235,237,114,254,176,87,183,49,76,134,220,219,79,226,82,227,196,
159,228,151,193,127,234,103,70,85,194,3,138,30,17,25,247,130,189,29,239,122,101,6,16,117,185,78,27,58,120,1,228,54,162,7,1,4,0,0,16,64,8,32,117,163,93,162,251,181,237,150,180,41,123,23,244,135,237,168,135,174,229,232,113,69,23,201,27,28,18,127,200,47,
129,238,217,210,253,210,35,236,20,43,119,159,15,93,128,62,104,115,31,113,124,142,36,5,19,101,216,95,6,152,186,18,19,40,220,145,144,94,87,119,149,94,215,116,117,11,214,135,149,27,66,76,0,89,219,222,11,32,183,18,61,8,32,0,0,128,0,66,0,169,27,237,255,116,
191,182,243,89,135,151,93,175,225,238,237,209,229,220,195,189,48,32,169,190,68,251,125,156,99,126,110,70,78,30,182,33,99,192,109,189,236,101,26,165,154,81,19,61,234,161,127,223,235,218,110,146,214,58,181,248,54,50,59,102,154,85,174,204,180,175,50,66,
136,14,60,157,214,180,243,46,127,21,209,131,0,2,0,0,8,32,4,144,131,223,82,149,124,165,163,50,64,57,86,57,93,89,160,20,42,69,202,50,101,145,50,83,153,162,140,82,186,43,173,28,187,9,225,22,199,239,72,207,203,187,216,48,80,106,244,67,79,179,26,112,107,111,
241,167,249,77,24,88,179,102,129,188,251,222,75,114,222,249,203,205,191,19,130,9,102,74,150,158,106,101,2,200,173,54,128,164,55,79,147,81,143,13,150,9,175,142,148,126,191,237,41,137,129,4,187,52,239,17,29,164,67,219,86,230,251,140,22,233,234,186,253,
237,40,73,169,16,162,71,99,58,44,109,235,5,144,155,149,54,202,48,101,178,251,24,231,42,139,221,199,185,216,125,204,103,40,39,184,143,177,155,210,78,105,172,196,19,64,0,0,0,8,32,4,144,232,219,97,202,36,101,163,114,187,242,178,99,119,9,223,233,141,44,236,
195,87,198,207,172,61,202,119,202,103,202,143,102,197,170,27,187,155,41,84,165,151,213,213,63,11,245,11,154,235,205,159,127,186,58,85,187,148,189,202,143,50,97,252,40,243,243,214,179,91,152,81,14,61,130,210,239,119,61,109,184,200,79,55,83,172,198,60,
59,76,210,91,165,185,215,63,85,246,236,249,82,190,249,246,61,25,53,114,136,249,89,179,19,26,201,56,117,221,145,143,238,91,212,62,225,165,17,210,106,70,115,239,120,119,150,251,24,203,183,91,249,94,249,159,187,140,239,45,202,57,202,68,247,60,18,64,0,0,
0,1,4,4,144,50,90,142,114,146,114,151,242,81,120,39,59,53,45,85,218,180,105,33,131,6,245,145,145,35,7,201,228,227,198,203,130,5,51,228,212,211,78,144,120,95,156,248,82,124,146,63,173,169,180,60,173,185,52,61,174,177,52,28,219,64,242,250,135,36,167,123,
64,2,157,178,37,181,81,138,196,165,196,153,159,13,123,104,160,45,12,15,31,133,80,129,162,239,255,245,48,247,21,204,204,150,207,62,127,89,157,170,111,100,199,143,111,170,175,59,228,222,7,110,54,191,75,235,148,106,194,202,132,127,143,50,83,173,244,207,
178,15,207,146,163,222,24,83,60,117,171,115,167,14,178,123,239,39,234,122,223,42,123,228,133,151,30,178,143,35,205,145,225,127,25,40,99,254,57,108,191,125,64,186,110,236,36,153,173,51,36,187,67,166,57,230,188,190,33,57,108,84,125,105,114,108,35,105,54,
165,137,228,79,111,42,109,206,108,97,238,75,223,86,239,222,221,100,230,172,147,100,220,248,225,50,108,216,0,243,239,166,77,27,73,66,66,124,233,112,162,207,227,3,238,168,80,19,2,8,0,0,32,128,128,0,98,167,14,157,173,188,227,117,156,27,52,168,39,227,199,
143,144,179,207,89,38,183,254,254,42,121,241,229,135,229,171,111,254,35,63,203,199,234,33,252,79,249,194,29,161,248,68,58,182,109,43,122,106,213,208,123,251,201,209,111,141,145,113,207,143,48,163,25,122,163,64,189,95,135,14,12,67,239,239,47,125,127,215,
83,134,253,117,160,140,122,124,200,190,211,160,30,177,211,160,242,79,107,102,238,91,7,27,61,234,177,243,167,119,84,0,217,166,190,255,84,62,252,232,121,105,16,172,39,78,162,35,3,239,232,107,2,71,183,139,58,155,203,215,27,24,146,241,47,141,148,172,158,
153,230,223,55,221,124,133,186,206,79,234,186,111,201,158,189,31,153,99,29,212,191,143,249,93,151,141,101,236,152,174,142,69,79,225,210,35,35,163,30,27,162,142,121,136,140,121,106,168,121,12,122,170,152,54,225,149,145,114,212,235,163,37,189,147,29,97,
121,242,169,63,139,109,159,154,115,240,211,158,247,229,127,159,189,44,79,255,243,62,249,237,77,151,201,250,13,133,50,97,194,72,105,212,176,65,120,24,249,82,249,141,210,139,0,2,0,0,8,32,56,84,3,200,44,229,61,111,26,213,152,177,67,229,250,27,46,145,119,
222,123,70,246,202,71,102,244,193,250,202,116,180,119,239,253,192,116,182,119,237,126,87,125,125,79,253,236,51,57,110,210,4,211,193,62,98,195,225,37,187,145,63,98,87,180,210,116,231,126,180,219,161,223,47,124,232,253,62,212,239,71,169,78,127,86,15,27,
32,254,124,223,239,212,237,126,103,194,199,15,59,223,86,199,241,161,236,217,243,129,140,30,49,216,252,94,175,136,117,204,187,99,165,179,187,116,174,222,87,100,216,95,6,138,147,164,194,72,48,87,62,250,228,5,115,92,250,186,63,236,212,1,230,7,89,127,86,
161,185,108,203,153,205,76,216,25,249,200,254,5,240,58,40,105,222,113,135,239,83,162,71,73,6,254,193,134,152,250,129,92,21,198,94,53,231,68,159,131,221,123,223,55,199,104,195,200,55,238,249,250,222,132,181,119,223,123,78,126,255,251,107,228,248,19,142,
146,140,140,180,240,105,94,23,184,117,53,4,16,0,0,64,0,193,33,17,64,130,202,239,189,79,231,117,7,249,201,167,255,164,14,239,75,183,3,173,191,126,108,130,198,143,187,222,113,59,243,251,210,63,215,65,225,202,171,206,55,183,209,240,200,6,17,109,48,184,223,
166,130,79,15,149,129,183,247,49,1,162,126,32,79,222,123,255,185,176,0,241,182,27,116,190,148,153,103,156,100,238,167,227,170,118,50,241,253,113,210,97,105,27,243,239,86,103,228,75,247,95,31,97,190,63,110,226,145,230,186,123,126,254,160,248,250,250,241,
92,125,205,70,243,251,70,147,27,148,236,41,18,197,49,142,127,121,164,28,190,210,22,170,79,57,254,24,117,155,95,203,158,189,31,148,123,94,118,237,126,207,156,63,59,82,244,189,185,252,235,175,63,33,133,133,179,37,43,43,195,11,34,175,184,5,236,4,16,0,0,
64,0,65,157,14,32,105,202,223,204,167,249,245,243,228,214,91,175,53,29,100,221,81,214,159,230,123,35,15,145,208,1,228,169,127,222,107,58,212,201,173,146,204,244,165,81,143,69,215,185,215,129,160,199,101,93,204,109,140,28,50,200,140,178,252,44,31,21,223,
199,46,19,64,190,146,69,11,10,204,101,218,23,182,54,1,68,215,100,152,127,47,105,45,109,22,180,52,223,95,114,241,58,243,56,244,244,45,125,221,29,63,218,99,188,229,214,43,205,239,243,70,134,76,13,72,52,1,196,236,35,162,142,177,222,184,92,115,27,151,95,
113,174,25,85,137,244,28,233,243,185,243,167,119,221,209,145,31,228,217,103,255,42,3,6,246,244,66,200,199,181,117,74,22,239,85,0,0,64,0,33,128,84,71,139,83,238,55,83,166,186,180,151,109,111,63,99,234,37,244,167,245,186,179,174,69,218,177,182,1,228,11,
249,244,139,151,229,176,64,125,179,87,199,192,63,244,54,171,81,69,19,64,244,168,73,215,11,59,153,14,249,73,39,78,50,183,25,62,130,97,71,64,182,203,188,57,167,153,203,116,88,222,70,38,190,59,78,242,79,106,106,139,206,215,117,144,252,211,237,247,183,220,
122,149,233,228,123,163,54,182,134,228,91,185,125,243,181,230,247,185,195,114,76,109,74,52,1,68,47,221,59,244,207,253,197,151,225,147,244,148,52,121,249,223,143,152,17,153,104,206,147,119,44,122,250,154,174,111,249,113,231,71,114,242,41,147,194,11,213,
15,35,128,0,0,0,2,8,234,98,0,209,251,117,72,139,150,205,84,248,120,86,29,206,78,83,39,17,205,168,71,184,189,63,127,104,194,193,209,19,70,219,34,239,243,58,202,248,151,70,68,31,64,46,232,104,174,63,253,196,227,212,237,125,190,79,0,209,163,50,186,190,98,
202,241,71,23,223,135,46,118,215,181,31,102,191,143,141,157,164,201,212,70,230,251,219,110,187,218,76,185,10,159,54,166,71,64,116,93,139,41,174,31,159,39,99,159,137,46,128,140,125,126,132,244,188,210,142,208,244,239,221,83,118,239,209,53,31,31,196,116,
190,172,183,204,57,251,89,62,51,171,104,213,214,29,216,121,175,2,0,0,2,8,1,164,170,77,111,176,183,51,41,41,81,30,123,252,46,51,242,81,94,125,71,164,108,29,200,15,178,102,205,34,211,145,110,126,114,19,187,202,212,163,81,4,144,23,70,168,16,97,3,200,41,
211,142,219,111,4,68,23,115,127,251,221,27,210,189,179,93,245,74,47,215,123,228,171,163,164,193,240,122,230,223,122,99,195,198,83,108,24,209,5,223,225,1,196,78,193,218,33,191,250,245,6,187,23,200,244,38,230,254,74,239,5,82,225,241,169,199,227,141,182,
172,85,143,83,79,165,242,166,120,197,202,142,204,236,144,15,63,122,65,26,53,42,94,41,107,8,1,4,0,0,16,64,80,151,2,200,197,186,95,121,250,140,19,205,180,36,93,96,30,237,148,171,178,167,97,125,35,15,62,116,171,122,96,62,73,110,151,108,151,179,125,98,72,
116,35,32,23,217,41,88,83,77,129,247,231,165,138,200,191,144,215,255,251,184,164,37,166,138,19,242,201,176,191,12,48,75,253,230,245,11,217,64,114,99,15,105,166,130,143,254,254,214,91,247,29,1,209,65,65,255,91,47,237,171,127,223,102,97,75,27,144,34,44,
148,55,43,120,61,61,76,82,218,165,152,235,63,242,247,59,76,141,73,85,207,153,169,109,217,253,174,25,129,58,247,220,229,94,0,185,145,0,2,0,0,8,32,168,43,1,36,93,121,51,33,33,65,158,123,254,65,211,137,142,117,218,213,254,1,228,127,242,201,103,47,74,195,
220,6,166,14,100,208,157,125,100,204,51,145,215,129,232,229,121,123,93,211,213,110,240,215,165,171,123,155,31,187,35,5,111,153,0,241,135,219,109,13,71,118,223,76,83,16,174,11,201,67,189,236,174,233,3,254,208,91,90,205,204,55,223,95,246,235,179,205,229,
189,17,138,159,213,177,237,250,233,61,105,147,111,11,214,123,111,234,102,70,64,34,63,182,97,210,255,214,222,102,137,226,6,129,122,242,249,151,255,54,129,168,58,206,155,29,5,249,90,94,125,253,49,73,79,55,75,244,254,87,73,33,128,0,0,0,2,8,234,66,0,25,97,
106,24,6,244,82,157,243,247,76,104,168,142,78,180,182,231,231,247,101,247,158,15,100,248,208,129,166,147,175,55,8,52,203,241,70,216,201,31,245,228,16,25,122,95,127,137,203,139,147,212,248,20,249,215,139,15,155,85,175,116,7,93,175,136,165,3,197,177,19,
199,153,219,110,187,184,149,28,249,234,104,51,194,146,211,211,6,16,189,132,111,55,183,136,125,242,177,19,36,124,25,94,61,61,236,169,167,255,108,126,231,15,250,205,200,199,152,167,34,12,71,234,178,19,94,30,41,29,150,183,117,111,123,188,84,180,252,110,
108,225,237,99,249,246,251,55,165,125,187,214,250,62,182,43,45,8,32,0,0,128,0,130,186,16,64,102,235,62,101,209,242,57,166,67,175,87,150,170,174,78,180,173,3,249,86,206,63,127,165,173,3,153,218,196,108,246,23,241,50,183,143,218,101,110,27,140,183,53,29,
23,92,176,74,221,222,94,247,118,247,200,214,23,30,148,4,127,188,248,83,125,50,228,79,253,236,70,130,127,31,44,193,46,1,91,24,126,83,79,25,254,208,64,179,75,122,118,74,166,188,251,193,179,102,90,216,222,159,237,70,138,83,79,156,104,46,215,226,148,102,
50,225,165,145,42,88,68,86,255,161,55,37,212,5,235,185,35,114,204,245,175,184,50,186,229,119,35,97,54,49,252,249,67,25,58,180,159,55,13,107,16,1,4,0,0,16,64,80,23,2,200,122,211,185,191,112,149,153,126,245,227,79,239,86,107,71,90,223,230,253,247,223,108,
58,209,129,94,89,102,231,240,136,11,189,31,25,100,234,50,186,185,117,32,77,27,55,146,79,63,215,83,157,84,172,249,126,155,244,235,103,247,204,200,63,73,5,27,21,32,204,110,234,91,6,73,86,27,187,161,95,223,223,246,148,35,95,27,45,185,195,108,77,200,252,
5,51,196,107,55,223,122,185,29,253,72,247,203,160,59,220,169,97,15,71,184,65,226,83,67,101,232,3,253,197,201,118,36,45,33,85,94,125,237,239,213,54,253,106,223,37,134,63,150,49,99,134,122,1,100,4,1,4,0,0,16,64,80,23,2,200,85,186,79,121,205,181,27,165,
244,50,181,213,19,64,182,203,59,239,253,83,178,82,51,197,73,242,201,224,63,246,147,49,255,28,26,249,52,172,199,245,104,195,112,9,245,179,211,170,122,118,59,66,46,190,228,28,233,209,205,174,124,149,213,46,67,70,254,109,176,217,195,67,95,118,216,3,3,36,
173,113,170,248,252,62,233,127,115,79,57,234,141,49,50,224,150,94,226,75,240,153,203,47,90,116,134,172,95,191,68,124,142,223,252,187,211,154,246,38,188,68,179,75,187,46,116,239,113,153,221,97,189,111,143,238,178,75,133,54,61,90,81,157,231,77,79,21,211,
155,20,246,235,215,67,223,207,158,218,180,41,33,239,85,0,0,64,0,33,128,84,165,93,161,251,148,87,95,179,209,142,128,84,123,0,249,196,212,149,12,232,221,203,116,216,187,255,170,179,93,109,42,210,253,64,30,30,100,54,48,28,242,199,254,18,60,34,219,27,13,
112,195,71,166,12,186,179,175,173,43,81,151,27,253,228,80,25,242,231,254,146,156,155,36,241,105,241,50,240,142,222,166,40,93,23,151,119,92,217,78,5,32,103,159,235,183,56,165,185,140,219,58,92,70,253,35,186,29,218,117,96,105,113,114,83,119,249,221,197,
98,151,223,173,222,145,35,189,196,240,55,223,189,33,29,59,181,211,247,243,189,210,129,0,2,0,0,8,32,168,11,1,228,74,27,64,206,175,145,0,178,107,247,123,230,118,231,204,57,197,116,216,91,157,153,175,2,200,200,168,246,219,24,254,240,64,25,251,236,112,83,
163,209,190,168,141,52,154,212,64,58,174,106,107,66,135,174,17,41,153,26,53,204,4,146,248,180,56,73,202,73,50,117,33,58,148,232,186,16,61,106,209,239,230,94,210,236,164,38,210,100,74,35,233,115,93,55,25,171,130,139,153,182,245,112,228,225,99,212,99,67,
204,104,75,90,187,84,243,120,254,250,240,31,204,200,81,117,158,51,47,184,109,255,250,117,105,221,218,172,210,245,177,210,144,0,2,0,0,8,32,168,11,1,100,147,238,83,94,127,195,197,53,18,64,188,29,199,239,184,243,55,166,195,158,217,43,195,132,2,211,241,143,
98,212,97,248,150,129,234,122,67,76,120,209,35,26,122,20,69,223,142,254,185,119,25,93,199,209,95,133,12,61,253,42,173,105,170,217,23,100,244,19,110,192,120,196,46,235,171,11,213,53,61,242,97,66,80,20,225,195,44,191,171,174,215,255,22,91,123,210,184,222,
97,213,186,252,238,190,231,236,115,121,251,189,103,36,148,19,244,150,225,77,39,128,0,0,0,2,8,234,66,0,217,168,251,148,191,186,116,125,13,6,144,207,229,191,111,62,110,10,182,157,128,35,195,238,239,47,99,158,26,26,85,199,63,188,48,93,143,104,148,85,179,
161,167,98,121,181,25,57,221,131,102,181,42,173,244,202,90,230,250,177,220,247,223,236,242,187,135,175,180,203,239,78,157,50,209,156,179,221,123,223,171,129,115,182,67,238,184,235,55,222,116,177,251,216,136,16,0,0,16,64,80,87,2,200,60,221,167,92,190,
98,174,84,247,50,188,30,93,160,253,253,142,55,165,103,183,46,182,144,252,138,46,251,76,157,170,46,227,254,53,92,186,156,223,209,220,71,94,255,144,25,49,41,29,64,170,66,239,126,174,87,241,202,29,97,87,213,186,230,26,91,184,255,195,174,234,61,95,187,205,
10,88,59,100,241,146,89,94,0,89,73,0,1,0,0,4,16,212,149,0,98,54,34,236,219,183,187,236,218,253,190,84,231,70,132,158,157,187,223,49,133,218,203,151,207,51,29,234,54,179,91,154,169,84,213,29,64,244,94,30,109,102,219,157,205,243,6,228,154,233,87,213,25,
64,244,242,187,195,30,232,47,190,28,71,18,124,9,242,202,171,143,168,199,245,101,181,158,43,187,11,250,151,242,217,23,47,75,179,102,141,245,99,249,73,233,78,0,1,0,0,4,16,212,149,0,162,107,11,254,235,247,251,229,241,39,255,104,62,121,215,157,224,234,236,
84,255,184,203,238,7,242,187,155,236,222,27,245,71,229,217,149,171,30,169,190,240,97,54,7,124,110,152,132,250,216,205,1,115,186,5,204,84,171,104,107,77,42,94,126,119,184,116,255,149,187,252,110,207,238,234,60,189,93,237,203,239,254,96,206,213,46,185,
252,138,115,188,209,143,135,156,90,214,120,175,2,0,0,2,8,1,164,90,54,35,60,242,168,81,234,48,190,146,221,123,62,172,246,16,34,242,181,188,250,250,163,146,28,151,44,254,44,191,12,123,112,64,236,117,32,229,20,135,15,254,99,95,73,200,76,48,29,247,228,188,
36,25,124,119,95,179,12,111,181,220,199,195,182,254,163,197,169,205,204,237,111,216,80,104,138,235,171,119,249,221,109,102,164,232,227,79,94,144,122,245,66,94,0,25,70,0,1,0,0,4,16,212,181,0,210,88,249,92,247,45,111,189,237,74,117,40,123,85,8,121,223,
116,136,171,47,128,252,79,246,236,253,80,186,182,183,53,26,125,110,232,110,86,179,170,150,112,240,200,32,57,242,223,163,164,67,97,155,146,125,62,124,142,244,184,244,8,59,213,235,225,234,25,97,25,253,228,48,73,111,159,102,110,255,254,7,110,174,214,162,
125,111,234,149,14,106,147,143,27,239,61,142,123,157,90,216,120,175,2,0,0,2,8,1,164,58,218,52,221,183,76,75,75,149,135,254,122,155,9,33,123,229,35,213,49,126,171,122,10,171,247,234,250,146,111,229,148,147,143,51,157,235,246,75,219,152,165,116,71,62,50,
184,74,193,192,238,17,50,76,134,255,101,160,164,55,51,225,224,91,229,111,182,14,36,100,54,13,28,245,196,144,42,135,16,61,253,106,192,109,189,205,177,55,205,107,40,223,126,255,95,169,142,229,119,189,145,38,61,242,161,71,159,230,205,63,213,11,31,239,185,
193,144,0,2,0,0,8,32,168,147,1,68,183,75,117,255,50,35,35,93,110,190,245,42,83,15,162,167,25,253,180,247,253,106,26,5,249,94,174,187,225,98,211,193,206,29,21,50,53,27,122,101,169,170,76,139,210,133,225,71,190,54,90,154,29,215,216,235,184,223,174,28,166,
124,160,255,221,121,125,7,57,234,141,49,102,207,144,170,140,176,76,120,105,132,116,92,109,118,37,151,73,19,199,155,192,176,231,231,15,170,86,156,255,147,46,206,215,59,197,255,36,223,126,187,77,78,57,117,178,247,24,126,80,250,59,181,180,241,94,5,0,0,4,
16,2,72,117,182,139,188,105,76,83,167,77,148,231,159,127,216,124,50,47,242,163,153,34,180,87,62,44,119,218,145,46,54,215,191,43,143,158,94,244,204,115,15,136,223,241,73,124,211,248,226,149,165,76,49,186,222,223,227,17,119,159,142,8,232,21,174,244,244,
42,189,169,96,155,185,45,195,59,238,29,194,71,116,252,73,126,105,191,164,141,140,125,102,152,76,248,247,72,83,43,162,235,66,198,60,61,180,76,163,53,117,76,58,176,152,13,19,213,253,232,203,235,17,144,224,160,108,115,63,191,253,221,101,38,52,232,37,139,
245,78,239,187,118,191,107,106,65,180,240,199,187,239,185,121,199,252,94,143,42,233,125,81,236,136,199,143,230,119,55,223,114,149,180,108,213,220,123,12,239,40,3,157,90,220,120,175,2,0,0,2,8,1,164,186,219,25,202,23,118,52,36,77,78,61,245,4,249,211,189,
191,149,207,191,120,213,237,60,239,112,125,239,10,255,190,172,127,123,246,152,16,211,171,139,221,15,100,192,173,189,101,226,187,227,204,84,44,109,156,246,47,107,188,251,117,220,11,195,203,48,194,132,130,129,127,232,45,13,199,53,40,169,251,176,161,35,
188,173,242,126,167,167,99,117,94,215,94,6,221,217,199,76,215,210,187,164,151,54,252,175,3,139,119,86,215,33,71,143,206,140,126,124,136,57,182,97,127,29,32,190,160,79,210,146,83,229,237,247,158,21,219,190,114,131,196,183,102,164,104,255,199,91,214,57,
250,84,190,223,177,77,94,120,241,175,114,193,5,171,205,242,199,97,199,255,87,165,173,83,203,27,239,85,0,0,64,0,33,128,212,68,107,170,92,161,124,233,117,144,155,54,107,44,99,198,12,149,53,107,23,201,165,191,62,75,46,188,104,181,92,112,161,114,193,42,57,
255,252,21,114,238,121,203,229,172,115,150,201,89,103,47,149,117,27,150,200,186,245,75,204,101,215,172,89,40,43,87,207,151,149,171,230,153,159,117,234,216,222,78,195,234,155,35,45,79,111,46,205,167,52,150,102,39,52,54,211,168,154,78,110,100,52,153,164,
28,171,28,211,208,104,124,148,114,100,67,105,116,228,97,210,104,124,3,9,118,205,150,248,212,56,175,227,254,137,114,116,57,143,227,4,229,223,222,99,72,204,74,144,180,38,169,146,222,52,85,210,194,232,127,235,26,146,244,22,105,146,217,58,67,178,218,102,
72,224,240,44,9,30,145,45,185,125,114,36,208,49,203,92,63,51,45,67,38,78,28,43,211,79,62,86,153,44,51,10,78,148,51,103,79,151,249,11,102,200,146,194,89,178,98,229,60,89,187,110,145,156,125,110,145,58,63,107,228,146,75,214,25,103,157,181,84,230,204,59,
85,70,142,28,36,237,219,183,150,164,164,164,240,224,241,79,229,100,231,23,210,120,175,2,0,0,2,8,1,164,38,91,27,101,145,242,184,183,82,86,45,242,190,114,165,123,140,21,181,160,114,186,114,183,242,150,123,189,247,220,58,145,143,220,0,163,31,219,118,183,
136,125,135,178,75,217,93,67,199,253,141,242,154,178,73,153,160,36,59,191,160,198,123,21,0,0,16,64,8,32,7,114,185,222,35,149,117,202,133,202,121,202,57,202,89,202,6,247,231,218,26,119,250,211,10,165,72,41,84,150,184,22,185,230,43,243,148,57,202,108,101,
150,107,166,59,5,108,134,114,170,59,50,112,146,107,170,59,205,234,68,101,128,146,19,195,99,200,84,66,238,117,245,215,122,110,209,186,126,108,205,148,86,238,52,168,78,74,23,165,167,210,87,25,164,140,84,198,186,231,224,88,101,138,50,221,61,214,51,221,199,
180,88,89,238,158,135,115,220,243,116,129,123,78,206,116,111,163,165,146,228,252,66,27,239,85,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,
0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,112,18,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,
0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,
0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,4,16,78,2,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,
32,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,
0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,0,0,0,
1,4,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,64,0,1,0,0,0,64,0,1,0,0,0,0,78,2,0,0,0,0,2,8,0,0,0,0,2,8,0,0,0,0,16,64,0,0,0,0,16,64,0,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,128,0,2,0,0,0,0,4,16,0,0,0,0,4,16,0,0,0,0,32,128,0,0,0,0,32,128,0,0,
0,0,32,128,0,0,0,0,0,1,4,0,0,0,0,1,4,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,8,32,0,0,0,0,80,19,254,31,186,211,221,255,42,1,230,166,0,0,0,0,73,69,78,68,174,66,96,130,0,0 };

const char* about_png = (const char*) temp_binary_data_7;

//================== add.png ==================
static const unsigned char temp_binary_data_8[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0\x18\0\0\0\x18\x08\x06\0\0\0\xe0w=\xf8\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0\x07\xd0iTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:"
"meta/\" x:xmptk=\"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl."
"org/dc/elements/1.1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmlns:tiff=\"http://ns.adobe.com/tiff/1.0/\" xmlns:exif=\"h"
"ttp://ns.adobe.com/exif/1.0/\" xmp:CreatorTool=\"Adobe Photoshop CC 2017 (Windows)\" xmp:CreateDate=\"2017-01-04T13:04:31+01:00\" xmp:ModifyDate=\"2019-01-27T14:10+01:00\" xmp:MetadataDate=\"2019-01-27T14:10+01:00\" dc:format=\"image/png\" photoshop:"
"ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpMM:InstanceID=\"xmp.iid:c43064d7-3a91-9040-a34e-80eb383c3c86\" xmpMM:DocumentID=\"adobe:docid:photoshop:b57f0b78-d276-11e6-86fd-f22ea20d4df0\" xmpMM:OriginalDocumentID=\"xmp.did:c47ec9ce-5"
"036-1546-8ca3-36273bed87f6\" tiff:Orientation=\"1\" tiff:XResolution=\"720000/10000\" tiff:YResolution=\"720000/10000\" tiff:ResolutionUnit=\"2\" exif:ColorSpace=\"65535\" exif:PixelXDimension=\"24\" exif:PixelYDimension=\"24\"> <xmpMM:History> <rdf:"
"Seq> <rdf:li stEvt:action=\"created\" stEvt:instanceID=\"xmp.iid:c47ec9ce-5036-1546-8ca3-36273bed87f6\" stEvt:when=\"2017-01-04T13:04:31+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2017 (Windows)\"/> <rdf:li stEvt:action=\"saved\" stEvt:instance"
"ID=\"xmp.iid:6286ab82-7d72-974d-b733-8603bb425782\" stEvt:when=\"2017-01-04T13:10:03+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2017 (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:c43064d7-3a91-9040-"
"a34e-80eb383c3c86\" stEvt:when=\"2019-01-27T14:10+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>*1`\x9d\0\0\x04\x9bI"
"DATH\x89\xb5\x96\xdb\xab\xddW\x11\xc7?3k\xfd~\xbf\xbd\xf7""9g\xf7\\\x92\x93\x93K\xb5\xa9V\x13%\x82\xc6\xb6\xd1\x06\x11\xf5""A\xbc\"\"\xf4\xd1\x0b\xfa \xfe\x01\xe2\xab\xef\x82\xf8*(\x88 *\"\xd8\x87""B\x83Tii\xaai\x89""F1i\xa8\xadMO\xea\xe9\xb9\xe5\xec"
"\xb3\x7f\xfb\xf2\xfb\xad""5\xe3""CZ0\xd9I\xdf""2O\xeb""a\xd6|\x98""5\xb3""f\xbe\xe2\xee\xdcK\xd3{\x1a\x1d\x88o\x1f\xdes\xf9\x1bt'=\x16\x86\x8b\xd4\xdd\x1a\xf1\x88\x9a\x94\xd3\xb2>\xae\x16\xcf\x04\x8f\x1f\x05V\x01q\xf1M\xc8\x17\xc0\x9e\x0f\xb9\xfb\xb2"
"\xa0\xd3\xd1\xdc\x06\x9b\x87.\x91\xe2\x18\xb5\xc8\xee\xd2\xdfo\x05\xdcn\x8e\xaf\x89\xeb\x97\x83\xc5\x87\xd5\xc3\x03\x8e\xdf\x9f<\xcf;H$\x0c\x03\xe1\x84#g\x81\x17\x80\xdf\x83_\x7f\xc7\x0cn\xb3\xf7\x02\x8f\x83|/xqHQZZ\xa6\x9ep\x9c \x8aR<\x94""1@\xbf\x84"
"\xcb*\xae\xbft\xb1\xab.\x19\x17}G\xc0<\xf0""C\x94\xaf\xb8S:N&\xa3\x08=\xad\x80\x9b\x85\xcb\xe4\xb7\xfdW\xc4\xc2\x0f""B\x8e\xa7\xc4\xe5\x9b\xc0\xde]3\x10x\xb7\x9a|]\xad\xfa\xa4\x85T\xb9&\x1cXow8\xd3{\x88\xaf-~\x9c""B\x94_\xdfx\x8eg\xeb\xcb\x1c)\x96pI\x92"
"C.\x8av\xe1\x13k\xeb\x8f}\xbf\x8d""7~\xea""2}\x99\xc5\xdb\0.^%\xcd\xa7\xdb\"}G\\\xd6\xd4""A\x04\xd4\x95\x9d\\\xb3R\xf4\xf9""B\xff#\x94Z\xf0t\xfd/v\xf2\x88""c\xc5""2\xaaN\xc6\xd1\\\x1c\x98\x1b\xae|;k\xefo\xa6\xf5\x1b\xc0\xe8\x16\x80Z8\xe9*\xa7\xa7U:\""
"\xe6$\xcb\x98""8\x86\x83\xb7$k\x98z\x8b\x1b\xb4\xd6\xe0\xde""2\xf1\x16qA\tDM4\xd5\xd6\x8a\xd3\x9cv\xf2\xab\xc0\xf9[\0\xdd\xf1\xc2\xa7\x94\xf0\xa1`\x01\xc4\xd8\xce\xbbl\xb4\x03\x1c`|\x8d\xad\xb9-\x06""6\xa1\x12""ck\xba\x05\xe3\xd7\xb8\x82\"\x08\xab""E"
"\x9f\xc3\xc5\x12""B\xc0\xads\n"
"\xb3\x9d\x19@\xf0\xe2\x94""a\xc7\xa7>\xc5q\xce.\x9c\xe4}\xbd""c8P\xa7mNT\xc7\xe9jID\xf9\xe2\xca\xc7xp\xfe \xbd""b\x99\x82\xc8K\xe3u\xfe""9\xfa\x0f\xe6\x10\x08\xef\x8a\xa2'g\x8a\xec\xe2\x8b\xc9\xd3|C\x8b;<\xd6=\xc1\xb7\x96?Mvc@\x83\xba\xd3}\xcb\xfd\xf3"
"\xfdG\xf8,\x8f\xb0 %\x1d-\xf9\xd9\xf6\x1f\xb9X\xff\x9b\xd6\x8dJ\xbc\x17""B\xec\xcf\x02\xec""f\xdb\xa9*\xee\xce\xfex\xc8\xf5\xed\r\xb2g\x06\xed\x88\xb9N\x8f\xb2\x7f\0\x11\xb8\xb1\x7f\x83z\\3\x8c=*\x89\x0c\xc6\xfb\x08""B\x10""A\x10\xdc\xf1\x19\x80M\xda"
"\x1c\x8a\x98\x8b^\x17o3\x7f\xd8\xbd\xc0S\x1b/\x82;\xa3\xd1u\xce\xae\x9e\xe1\xbb\xbd\xafRh\xe4\xe7\x1bO\xf2\xa7\x8dg\xe9t\x8f\xa2\x02\x93\xd2\xa8:\x05\x94\x8aO\xb2Y\x93l\x06\x90\xc7\xcd\xb5\x88\x1c\x8b\xd2{\xd0\xc4Yg\x97-\x19\xe0\xe2\xb4\xf2\n"
"+z\x1cQ!h\xe0\n"
"or\x81+\x14\xda \x08+,pH\x96\xd1\x18h\xdb\xe9n\xae\x9b\xff\xce>Q3=oA\x96\xac\xb1GM`un\x99\xb5p\0\x13\xe7\xd2|\xc9j\xff""0\xbdPQId\xb5\x7f\x98\xc8\xfb\xf9`\xe7(\xea\x8aY\xc6\xdd\xb0VH\xa3\xf4R\x1a\xb6\x7f\x9d\xfd\xc9i\xfc""4c\xbd_JC\xfb\x11\xf7LnZL\x04"
"\xda\x84\xa7LN\x19""C\xf0\x9c\xa1MdM\x98\tR\t\"\x01\x19\x08iO.6#97\x0b""0\xdbH\xb9~&\x8d\xe5\xdc\\q\xe8""3\xda+\xc8U\x86\x89s\xb4\\\xe6\xdah\x93\x1f\xbd\xf2;\"\xca\xab\xf2\x06G\x8a%\xc4\x80\n"
"\x82\x96x\xed\xd4\xc3\xed?7\xd3\xf4\x8c""e}}\x16 B\xa6\xfdG\x9b\xf6~\x1c\xea\xb0Pr\xdf\xc3\xd2)Uc\xe2`\\ds\xb2\xcfo\x86\xe7\x11\x84\xfe\xfc\x02\x07;\x8b\xa4\x9c\xc1\x03""6j\xbd\xd9\x1f\xbe""0i\x06?q\xcaK\"%\xb3\0@\x88""C\xc4\xce\r\xf3\xfa\xb1\xb9\x1d"
"\xef\xf6\xaa\xb5\x0f\xd8r\x0e\xee&\xb1*\x98/\xef\xbbyI\x8a\xb7\xfaP\\wb\x9eN\xb7\xaf\xd6\xbc\xf9\x0b\xa4\xfb\x84H\x98\xfc\x7f\xcc;\x8d\xeb\t\"\xbf\xcd\xa3\xe9\xeb\xcd\xde\xe0q\xb7\xe6sZ\x15K\xc4@\x11\x01\x07\xac\xa5m\x1b\xf2$\xed\xe9\xf6\xf4\xc9\\4\xbf"
"\xa2/\xcf\x81Ln\x0fv\xc7\x85#\xa2[d\x7f\"\x8d&\x9bi0\xbe,Ex4\x86\xe2\x01\r\xa1\x87""C\xf2""4\xca)\xbd""fS\x7f\xbe\x18u\x9e\xf2y?/\"w\x94\x0fw]\x99""7g\xb5\xbc\x98-mH\x93\xaf\x8a\xd8\x87]\xe2\n"
".n\xd2\xec""fO\x17\xdd\xf5/\x04\xae\x89\xca]\xa5\x89\xdck\xd9\xf2?+iWPrc{^\0\0\0\0IEND\xae""B`\x82";

const char* add_png = (const char*) temp_binary_data_8;

//================== connected.png ==================
static const unsigned char temp_binary_data_9[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0\x18\0\0\0\x18\x08\x06\0\0\0\xe0w=\xf8\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0\n"
"6iTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.or"
"g/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmlns:stRef=\"http://ns.adobe.com/xap/1.0/sType/ResourceRef#\" xmlns:dc=\""
"http://purl.org/dc/elements/1.1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:tiff=\"http://ns.adobe.com/tiff/1.0/\" xmlns:exif=\"http://ns.adobe.com/exif/1.0/\" xmpMM:DocumentID=\"adobe:do"
"cid:photoshop:cc9f056c-2c7e-704d-a9ad-900d10744c3f\" xmpMM:InstanceID=\"xmp.iid:ad956041-51d6-a147-8976-f4cc28c0cf87\" xmpMM:OriginalDocumentID=\"804224AE14CDA86C61B4F52790E3E185\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfil"
"e=\"sRGB IEC61966-2.1\" xmp:CreateDate=\"2019-01-27T13:40:59+01:00\" xmp:ModifyDate=\"2019-01-27T13:53:54+01:00\" xmp:MetadataDate=\"2019-01-27T13:53:54+01:00\" tiff:ImageWidth=\"1500\" tiff:ImageLength=\"1225\" tiff:PhotometricInterpretation=\"2\" t"
"iff:SamplesPerPixel=\"3\" tiff:XResolution=\"300/1\" tiff:YResolution=\"300/1\" tiff:ResolutionUnit=\"2\" exif:ExifVersion=\"0221\" exif:ColorSpace=\"65535\" exif:PixelXDimension=\"1500\" exif:PixelYDimension=\"1225\"> <xmpMM:History> <rdf:Seq> <rdf:"
"li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:51b7709a-3e12-ee44-8d84-5ee2a09b9357\" stEvt:when=\"2019-01-27T13:45:25+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"converted\" st"
"Evt:parameters=\"from image/jpeg to image/png\"/> <rdf:li stEvt:action=\"derived\" stEvt:parameters=\"converted from image/jpeg to image/png\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:af85123e-f6f3-184a-8aab-b51a2801c3ba\" stEvt:wh"
"en=\"2019-01-27T13:45:25+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:ad956041-51d6-a147-8976-f4cc28c0cf87\" stEvt:when=\"2019-01-27T13:53:54+01:00\""
" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> <xmpMM:DerivedFrom stRef:instanceID=\"xmp.iid:51b7709a-3e12-ee44-8d84-5ee2a09b9357\" stRef:documentID=\"804224AE14CDA86C61B4F52790E3E185\" st"
"Ref:originalDocumentID=\"804224AE14CDA86C61B4F52790E3E185\"/> <tiff:BitsPerSample> <rdf:Seq> <rdf:li>8</rdf:li> <rdf:li>8</rdf:li> <rdf:li>8</rdf:li> </rdf:Seq> </tiff:BitsPerSample> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>\xed"
"\xe7L:\0\0\x05\x0eIDATH\x89\xb5\x96YlTU\x1c\xc6\x7f\xe7.3w:\x0bS\x98\xb6ST\xaa@\xa7\x15\x14#\x88\xa8m\xa3\xa0\r\x04""5\xd1\xc4\x18\xb7\xb8\xe1\x02./\xc4\xf8\x02\x89\x0f\xe2\x9b\x89\xc4\x88\x1b\x9a\x18\x8d""b\xdc\xde\x14""d\x11\xa5\x8d\x82KD\x01\xe9\x82"
"\xd4\x82\xb4\xa5\x9dn3s;s\xef\xdcs\x8f\x0f\x03""c\xcb<\xf3\xdd\x97\xfbp\xce\xff\xbb\xdf\xf7\xff\xce\xff\\\xa1\x94\xe2""bB\xbb\xa8\xd5\x01\xe3\xfc\x8b-\x1d\x06\xecQ\xfe\x9c""8E\xc8\x08ru|\x1e\xef\x9e\xd8\xbb*\xa0\x1b\xb7w\x0cw\xb5\x8e:\x99\x94\xab""d\x08"
"\x10\xa6\xd0\n"
"\x89@\xac\xb7\xa5\xa6\xb9\xa3\xe0\xbb;\x9f^x\xdb\x9e\xc3\xe3\xfd*'\x1d\x96\xc4.c^$A\xdc\x0c\xcf$8\x0f\x85\"lX\x97l\xeb\xdd\xf5\xf2\xe7\xa7\x0e\xde""3\x98\x9f\x88V\x19\x01""f\x07\"\x84\x8d \0S\x9e\x1b\xf8}\xaaoY\xe7h\xd7\xb2\xa4\x15_/\x95\xff""E{r\xc9"
"\xe6\xac,\xf4_XO\x9c\xef\x81-\x1d\xce\xd8\xa3\xf4\xe7""F\x9a\xb6\xf6|\xf3\xfe\xde\xb3GZ\x92V\x9c\xd9\xc1\x08\xba\xd0\xf0|\x1f""Ei\xad&\x04\x86\xd0\xf1\x94\xcf\x98\x93""c\xa8""0\xce-u\x8b\x0f=\x9bZ\xf3""dC(q\xa4""a\x9a\x82r\x0f\x04\x82\x98\x11\xaa}\xbd"
"w\xd7[\xfb\x86\x8f\xb6\xa4\xa2s\x99\x13\x8c\xe2+E\xd1\x97\x80""B\0\xba\xd0\xc8zy\xfa\xa7\xd2H%\xa9\xb5""b4F\xe7r`\xe4\xf8\x8a""7\xbawn\x8f\x99\xa1+\x04\xa2\xac\xa0L\x10\xd0\x0c\xfd\xdd\xbe}/\xed\x1a:\xbcra$\x89!4\xa4\xf2\x11\xe7\x1e[:8\xbe\xc7pa\x92j"
"3\xcc\xc3\rm\x04""5\x93\x9c\x97'\xa0\xe9,\x08\xd7\xb1\x7f\xf8\xd8\x8a\xed'\xf7m\xb6\xf4\x80QA\xf0\xdb\xd8\xc9\xd6\x0f\xfe\xde\xff@2\x18'\xa8\x99\xf8(\x04\x02M\x08\xc6\xdd\x1cJ)\x86\x0b\x93\x08\x01\xdb\xaf_\xcf\x8b\x8b\xee\xa2\xa8$C\xf9I\xf2\xd2%\xa4\x07"
"\xa8\xb7\xaa\xf9\xa8\xef\xc0\xfd\x07\xd3=\xab*R\xd4""9|\xbc}(\x9f\x89\xa7""bI<%\x01\xd0\x85\xa0\xcf\x1e\xa1>T\xcd\xb6\xeb\x1eg`j\x9c:k\x16\xb5V\x9c\xf5\xbf\xbeOwf\x80\x9a`\x14""AI\xed\x9c`\x84\xee\xcc`\xa8s\xa4\xbb\xfd\xe6\xda""E\xbbg\x12\xa4\xbbn1u\x1d"
"C\xe8\x14\x95""DCc\xcc\xb5\x99""e\x86x{\xf9\x93\xb4\xd5""4\x97}]\xfb\xdd\xcb\xec\xec\xddM\xe3%\xcb\xd8\xba\xf4\x11\xba\xb3\x83l9\xfa%\x0b\"\xb5X\xbaIg\xbake\x85""E\x83\xf9\x89""E\xd5\x81*$>\x02p\x94G\xb6X\xe0\xd9\xd4\x9a\x19\xc5\x1d\xdf""cE\"\xc5\xdc"
"\xd9\xf3ym\xe9\xc3\xac\x9d{-\x7f\x8c\xf7""a\xea\xa5T\xc5\xcd*F\x9d\xec\xfc\n"
"\x8b\n"
"\xb2\x18\x8b\x1aU\xf8\xaa\xe4\xbd#]\xc2""f\x80\x95u\x8bg\xe4:\xa8\x19l\xba\xfa\x1e\x9eJ\xad\xa6(%ww\xbc\xca\xf7g\x8fqE\xa4\x06\xa9|\xc2\x86\x85+\xbdX\x85\x02\x81(\xc2\xcc\xb9\x14\xd0\x0c\xc2\x86\xc5\x85""0\x84N\xbd\x15\xe7X\xe6""4\x07\x86\xff\"a\xc5\xf0"
"\xa7m\x15\xe2\xff""Be\x02\xcb""0&3^\x1eM\x08@a\n"
"\x9dq\xd7\xa6;3PAp\x1e\xab\xea\xae""bI\xbc\x01\xc7/\xa2\xce\xa5\xce\x96\x0e\xa6""0r\x15\x04I+\xde""3\xe9N\xa1\xa3\x01\x02K7\x19-\xe4x\xe5\xe8W\xa4\x9dlE\xf1\t\xd7""f\xfd\xcf\xdb\xe9\xc9\x0e\x10""7\xabJ\xca""4\x8dq7G\xc2\x8a\x9e\xa8 hM4\xffP\xf4%\x9e\x92"
"H\xe5\x93\xf3\\\xb6\\s\x1f""7\xd7]\xc9""c\x07\xdf\xe4\xdb\xc1\xc3\x8c\xb9""9\x86\x0b\x19\xf6\x0e\x1d""a\xdd\xa1\xb7\xd8\xd1\xdfI\xdc,\x8d\x12\0_\xf9\xb8\xd2\xa3%\xd1\xb4\xbflg\x99\xa0\xf6\xca\xefj\x82\xd1\xe7\xc7\x9c\xdc\xac\xb0i\x11""2L6-\xbe\x9b\xdf"
"\xc7\xfb\xb8""a\xf7&\x0e\x8d\xf6\xd0<\xebR\xa4\xefs:\x9f\xc6\x95\x1eM\xd1ztMG*\x1f""C\xe8\xa4\x9d\x0cs\x82Q\xbb\xb5\xa6yO\x85\x82""e\xb3\xe7w\xacK\xdd\xfa\xe9Y7\x83\x86@\x17\x82\xfb~\xdc\xca\x86_\xde""c^U\x82\xea@\x94Sv\x9a""3\xf9""1,-@\x9d\x15G\xd3J"
"\x07LC\xe0*\x8f\xc1\xfc\x04\x0f]\xde\xb6\xe3\xc6""D\xaa\xac\xa0<M\x0b\xaa\xc8h>\x93\\w\xf0\xed\xcf\x0e\xa4\x8f\xb7-\x88$\xc9zy\x02\x9a""A\xc4\xb0\xf0\x95*O\xd3R\x0cJ\xd0\x85""FQI\xfe\xce\x0eqS\"\xd5\xf1\xce\xf2\xa7\x1f\xac\xb1\xa2\xa7""c\xe7\xfaRV\xa0"
"Pd\xa5""3\xf4h}\xcb""3m\x89\xe6\x9fN\xd8""C\xa0\x04\x11\xc3:\x97\xac\xe9\x10\x18\x9a\x8e.J\xa7\xbd""73\xc8\x8d\x89\xa6\x9f""6,\\\xfd""D\xd6\xcb\x9fV\xd3\xe2\xae\xcd\xdc\x06Y/\x7f\xf4\x85\xe6;\xef\xdd\xd8t\xc7\x87\xba""F\xa1+{\x86\x7f\xec\x11\xa6\xa4S"
"^gK\x87~{\x84\xae\xec\0\x02\x9c\xe7\x1a\xd7|\xb2\xb1\xe9\x8e\x07""2\xdeT\xcf\x85\x9fRq\xa3\t\x04\xb6\xe7\xfc\xbb\xa1\xb1\xfd\x11\xa5\xfc\x8f\x83\x9a\xb9\xb6s\xa4\xbbu\xac\x98[X\xf4\xbd*@\xc5\xccP\xa1\xa1j\xce\xc9\x9bj\x9a;\xf3\x9e\xfb\xf5\x86\xc6\xf6"
"=\xbf\x8d\xf5\xc9\xe9\xf7@E\x0f.\x16.\xfa_\xc5\x7f\xae\xe8""9sg\xfd;Y\0\0\0\0IEND\xae""B`\x82";

const char* connected_png = (const char*) temp_binary_data_9;

//================== default.chalayout ==================
static const unsigned char temp_binary_data_10[] =
"{\r\n"
"  \"mainLayout\": {\r\n"
"    \"type\": 1,\r\n"
"    \"width\": 1920,\r\n"
"    \"height\": 997,\r\n"
"    \"direction\": 2,\r\n"
"    \"shifters\": [\r\n"
"      {\r\n"
"        \"type\": 1,\r\n"
"        \"width\": 1920,\r\n"
"        \"height\": 997,\r\n"
"        \"direction\": 2,\r\n"
"        \"shifters\": [\r\n"
"          {\r\n"
"            \"type\": 1,\r\n"
"            \"width\": 1920,\r\n"
"            \"height\": 639,\r\n"
"            \"direction\": 1,\r\n"
"            \"shifters\": [\r\n"
"              {\r\n"
"                \"type\": 1,\r\n"
"                \"width\": 269,\r\n"
"                \"height\": 639,\r\n"
"                \"direction\": 2,\r\n"
"                \"shifters\": [\r\n"
"                  {\r\n"
"                    \"type\": 0,\r\n"
"                    \"width\": 269,\r\n"
"                    \"height\": 325,\r\n"
"                    \"currentContent\": \"Modules\",\r\n"
"                    \"tabs\": [\r\n"
"                      {\r\n"
"                        \"name\": \"Modules\"\r\n"
"                      }\r\n"
"                    ]\r\n"
"                  },\r\n"
"                  {\r\n"
"                    \"type\": 0,\r\n"
"                    \"width\": 269,\r\n"
"                    \"height\": 308,\r\n"
"                    \"currentContent\": \"Command Templates\",\r\n"
"                    \"tabs\": [\r\n"
"                      {\r\n"
"                        \"name\": \"Command Templates\"\r\n"
"                      },\r\n"
"                      {\r\n"
"                        \"name\": \"Custom Variables\"\r\n"
"                      }\r\n"
"                    ]\r\n"
"                  }\r\n"
"                ]\r\n"
"              },\r\n"
"              {\r\n"
"                \"type\": 0,\r\n"
"                \"width\": 1183,\r\n"
"                \"height\": 639,\r\n"
"                \"currentContent\": \"State Machine\",\r\n"
"                \"tabs\": [\r\n"
"                  {\r\n"
"                    \"name\": \"State Machine\"\r\n"
"                  },\r\n"
"                  {\r\n"
"                    \"name\": \"Dashboard\"\r\n"
"                  },\r\n"
"                  {\r\n"
"                    \"name\": \"Module Router\"\r\n"
"                  }\r\n"
"                ]\r\n"
"              },\r\n"
"              {\r\n"
"                \"type\": 0,\r\n"
"                \"width\": 456,\r\n"
"                \"height\": 639,\r\n"
"                \"currentContent\": \"Inspector\",\r\n"
"                \"tabs\": [\r\n"
"                  {\r\n"
"                    \"name\": \"Inspector\"\r\n"
"                  }\r\n"
"                ]\r\n"
"              }\r\n"
"            ]\r\n"
"          },\r\n"
"          {\r\n"
"            \"type\": 1,\r\n"
"            \"width\": 1920,\r\n"
"            \"height\": 351,\r\n"
"            \"direction\": 1,\r\n"
"            \"shifters\": [\r\n"
"              {\r\n"
"                \"type\": 0,\r\n"
"                \"width\": 255,\r\n"
"                \"height\": 351,\r\n"
"                \"currentContent\": \"Sequences\",\r\n"
"                \"tabs\": [\r\n"
"                  {\r\n"
"                    \"name\": \"Sequences\"\r\n"
"                  }\r\n"
"                ]\r\n"
"              },\r\n"
"              {\r\n"
"                \"type\": 0,\r\n"
"                \"width\": 1253,\r\n"
"                \"height\": 351,\r\n"
"                \"currentContent\": \"Sequence Editor\",\r\n"
"                \"tabs\": [\r\n"
"                  {\r\n"
"                    \"name\": \"Sequence Editor\"\r\n"
"                  }\r\n"
"                ]\r\n"
"              },\r\n"
"              {\r\n"
"                \"type\": 0,\r\n"
"                \"width\": 400,\r\n"
"                \"height\": 351,\r\n"
"                \"currentContent\": \"Logger\",\r\n"
"                \"tabs\": [\r\n"
"                  {\r\n"
"                    \"name\": \"Help\"\r\n"
"                  },\r\n"
"                  {\r\n"
"                    \"name\": \"Logger\"\r\n"
"                  }\r\n"
"                ]\r\n"
"              }\r\n"
"            ]\r\n"
"          }\r\n"
"        ]\r\n"
"      }\r\n"
"    ]\r\n"
"  },\r\n"
"  \"windows\": null\r\n"
"}";

const char* default_chalayout = (const char*) temp_binary_data_10;

//================== disconnected.png ==================
static const unsigned char temp_binary_data_11[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0\x18\0\0\0\x18\x08\x06\0\0\0\xe0w=\xf8\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0\tUiTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:meta/"
"\" x:xmptk=\"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns"
".adobe.com/xap/1.0/sType/ResourceEvent#\" xmlns:stRef=\"http://ns.adobe.com/xap/1.0/sType/ResourceRef#\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:tiff=\"http://ns.adobe.com/tiff/1.0/\""
" xmlns:exif=\"http://ns.adobe.com/exif/1.0/\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmpMM:DocumentID=\"adobe:docid:photoshop:bb3e1466-220f-3648-aa0d-27af66699334\" xmpMM:InstanceID=\"xmp.iid:6e369f37-3e44-0845-bfbd-52ce04da17d1\" xmpMM:OriginalD"
"ocumentID=\"804224AE14CDA86C61B4F52790E3E185\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"\" tiff:ImageWidth=\"1500\" tiff:ImageLength=\"1225\" tiff:PhotometricInterpretation=\"2\" tiff:SamplesPerPixel=\"3\" tiff:XResolu"
"tion=\"300/1\" tiff:YResolution=\"300/1\" tiff:ResolutionUnit=\"2\" exif:ExifVersion=\"0221\" exif:ColorSpace=\"65535\" exif:PixelXDimension=\"1500\" exif:PixelYDimension=\"1225\" xmp:CreateDate=\"2019-01-27T13:40:59+01:00\" xmp:ModifyDate=\"2019-01-"
"27T13:53:39+01:00\" xmp:MetadataDate=\"2019-01-27T13:53:39+01:00\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:53ab89fa-9ed0-d445-b137-d1858c8bf83c\" stEvt:when=\"2019-01-27T13:53:39+01:00\" stEvt:softwareAgen"
"t=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"converted\" stEvt:parameters=\"from image/jpeg to image/png\"/> <rdf:li stEvt:action=\"derived\" stEvt:parameters=\"converted from image/jpeg to image/png\"/> <rdf:l"
"i stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:6e369f37-3e44-0845-bfbd-52ce04da17d1\" stEvt:when=\"2019-01-27T13:53:39+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> <xmpMM:Der"
"ivedFrom stRef:instanceID=\"xmp.iid:53ab89fa-9ed0-d445-b137-d1858c8bf83c\" stRef:documentID=\"804224AE14CDA86C61B4F52790E3E185\" stRef:originalDocumentID=\"804224AE14CDA86C61B4F52790E3E185\"/> <tiff:BitsPerSample> <rdf:Seq> <rdf:li>8</rdf:li> <rdf:li"
">8</rdf:li> <rdf:li>8</rdf:li> </rdf:Seq> </tiff:BitsPerSample> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>jm`\xd7\0\0\x05=IDATH\x89\xb5\x96ml\x1c\xd5\x19\x85\x9f;sg\xf6\xc3\xb1\xf7\xc3\x1b\xdb\xeb\x18L>\x1c[@A@\xa1\x8d\xdaJ!\xc6"
"jR\x07\xa9? j*\xa0\xa8N)-\x02\x12""A*\x14\xd5\x14\xd5.RB\x13\xfeU\xc1\t\x02\x8c)\xc5""DUU\xa9\x15i\x90S\x85""4\xaa\xda*\xa5\r\xa2Ic\xc7\x8e\x9d\x92x\x1d{\xfd\xb1\xbb\x9e\x99\x9d\xb9\xb7?\xd6\xd9\xb2\x98\xfe\xcc\x95""F\x1ai^\x9d\xf3\x9e\xf3\x9e\xf7j\x84"
"\xd6\x9a\xeby\x8c\xeb\x8a\x0e\xc8\xff\xf7!\x98\x9dk\x98<\xd8\xf7""D\xf6\x0fG7\xb9\x17\xc7\xdapuRH\tB\xa0\x8b>X\xe4\xac\xe6\xa6\x8f\x13\x1b;\x86\xea\xbf\xdfu\xd8J7\x8c|\x1e\x8e\xb8""f\x91\xd6\x1a!\x04\0\xf3'>\xf8\xfa\xd8\xae\xdd\x07\xf2\x1f\x9e\xbe\xd9"
"\xb4#\x88h\x04""3\x14""A\x84""B\xa5\xda\xa2O\xb0\x98G;\x0e\xbe\x93'\xda\xdar\xbe\xf9\xa5\x9f\xff""8\xd1\xb9""e\xd0W\n"
"|\x1fi\xdb\x9fO0\xf3\x9b\xdf>8\xbc\xa3\xeb\x15""5SH\xd8Mi\x84i\x81\0\xb4.=\x15\xed\t\xb4\n"
"(^\xb9\x82\x19\xb1\x16\xea\x0f\xec\x7f""2\xf8\xce#o\xd8SS464T\x12\0\xcc\xfd\xf9\xaf_;{\xff\xd6_\x1bN\x90\x12\xa1""0\x04\x01""2\x11\xa7\\#\x04""B\x83?\x9b%\xc8\xe5\xb0\xea\xea""1\"aP\x8a ;CQ\xb9\xf3""5\x07\x0f\xed\xa8\xdb\xfe\xe0\xbb\xa9%\xcc\xf2\x90\x83"
"\\.9\xfe\xf4\xce""C:;\x9f""2\xe3""5\xc4\xda\xef%\xd4\xb2\x16\xf7\xd2""8\x02""A07K\xb0""0\x87{\xf9?\x84W7s\xe3\x8b=\xd8""74\xe2\xcf""eA\x08\x8c""D\x02\xd3\xd5""5\xee\xf3\xdd{Wd2\xcd|\x96`\xea\xd5\xfe]\xb9\xbf\x9fn\xb5\xea\x1b(NO\x11\xdf\xdc\xc1-CG\xb1"
"\x9bW\xb1""0v\x16Y\x9b@y\x0e\xb2\xa9\x81uo\xf7\x93~\xe6i\xb4""e\xe0^\xbd\x8c?;\x8b\xc0\xc0\xac[\x89w\xe1\xc2\xea\xec/\x0e?\xbb,E3\xbf\xff]\xbb!$\xa6\x1d\x82\xeaZFw?\x83L\xd5r\xcb\xd0qF\xf7<G\xc3\xa3]h\xd7""E&\x13\x98\xb1""8\xc3\xdf\xfa""6\xf3'O\x10J\xa6"
"1c5h\x15 \xa4\x05v\x94\xab\xc7\xde\xfbj\xfa\x85=\x95""38\xbd\xbe""5\x1bLe\xe3""2\x91\x02!(^\x9d\xa4\xe8\xe6h\xed\xef'\xb5m[\xc5l\xcf=\xfc(c\x03oP\xdb\xd8\xc8\xbaW^\x05\xc3\xe0\xdf\x8f<\x84\xac\x8a\xa1\xf2yDU\xd8\xbb\xeb\xe2\x85P\x85""E\x14\x8aq#\x14\x01"
"\xad \xf0\xb1S\r\xd8\xd1""8\xe7\xb6o\xe7\x93\x03/\x97\xcb\xb4\xef\x93\xe8h\xa7vm+7\xed\xdbO|\xeb""f\xe6?8\x01\x8e\x87@`\x86#\x88\xc0\xb0\x97Y$\xa4U\x92\xa8""5\x08\x83\xe2\xf4$\xa1\x96\xb5\xac\xeb>\xccho\x0fn&\xc3\xea\xde^\x84\x94\xd4=\xfc\x10\xc9-[\xc0"
"\x92\x8c|\xf7""12o\xbeN8\xdd\x0c\x86QJ\xda\xa7\xd4\xfeO\xc1g\"\xae\x03""E\xb0\x98\xa7\xfa\xcb\x1bh;\xd4\xc7\x95#G\x18\xed~\x1e\xed\xfb \x04re\n"
"\xf7\xfc\x08\xd9\xf7\x8f!\xab\xe2\x08)\x97\x9a\x13\x15""8e\x02\xed\xfb(\xc7)\x17\x18!\x9b\xe2\x95\xcb\x14>:C\xf4\xf6\xdb\xb9\xed\x9dw\x98|w\x90\xb1=\xdd\xa0J\xdd\x84[[\x88m\xf8\n"
"\xca\xf1\xd0""AP\xc2)\x16""a\xe9\xbdRA\x95\x9cV\xee""b\x89@\x80\x11]\x81;=\xcd""DO/A\xbe@\xd5]wr\xdb\xe0 \xd3""C\xc7\xb8\xb4w_)\xda\xaf\xbd\xc9\xd4\x91""A\xacT\xb2""d\x8f""a\x10""8\x8bhCy\xcb\x08\"k\xd6\x9dU\xf9""E\xd0\x1a\xed\xb9\xa0|n\xdc\xb9\x1b\x19"
"\x8f""3\xb2\xe3""1\x16N\x9e\"z\xe7\x1d\xdc:0\xc0\xa5\xde\x9f""1\xd9\xdfO\xcd\xa6\x8d\x84[\xd7\xe3""f>A\x18%(\x95/`\xadj\xfc\xd7""2\x82\xc4\x96\xce\xe3J\xfbZ\xfb~\xe9\x1aH\xd7\xd3\xfc\xf2>\xd2O=E\xe6Woqn\xdb\x03\x9c\xed\xf8\x06\x13\xdd=X\xd5q\xcewu\xe1"
"\x0c\x0fs\xeb\xd1\xf7\xa8\xbe\xe7n\xfc\x99it\xe0\xa3\x9c<\xf1\xf6\x8e?\x95\xc3sm\x0f\x82\\.\xf9q{\xc7\xa9\xc2\xe9\x0f[\xadt\x13""f4\xc2\x8a\r_\xa2\xf0\x8f\x7f\xe2\x8e_\x02\xadP\x85""E0L\xec\xfa:\x8a""3WQ\xbe\xc3\xfa\xb7\xde\xc6\x19\xb9\xc0\xc4O^@;\x1e"
"\xd6MM\xa3_8u\xf2^ke\xea""b\x05\x01@\xfe/\x7f\xdbtfk\xe7\xa0\xb9\x18\xd4\xca\xda\x14\xc5L\x06""3\x1a\xc1\x8c\xc5J\xd6)\xb5\xd4\x96@\x18\x06""A>\x87r<d2A0\x97\xc5W\xde""B\xdb\xc0/\x7f\x18\xef\xdc<\xb0\xcc\"\xa5\x14\xe1{\xbe""8\xb4\xa6\xaf\xef\x07\xd8""b"
"\xde\x9d\x98@\xa6\x92\x18\xd5""5K7ue\x8e\xb5\xd6\x98U+0\xaa\"x\xe3\xe3\x04\x81\x97[\xf3\xd2\xfe\x1f\xc5:7\x0f|\xba\xb6\xac \x9f\xcb!\xc3""aBR2\x7f\xfc\xc4\x03\xa3;w\xf5\x14>:\xd3""f\x84\xa2\x18\xd1\x08\xa6\x15""F\x84J\x0b\xaa=\x8f\xc0u\xd0\x8e\x8b\xbf"
"\xb8@\xb4\xad\xe5|\xf3\x8b{\x7f\x9a\xf8\xe6\xfd\x03""E\xa5\x10J!\xa5\xac$\xf0<\x0f\xdb.o8\xfeL\xb6i\xf2`\xdf\xe3\xd9\xf7\x8fn\xf2\xc6.\xde\x8c'bB\x9a\xe5\x9d\xc1""b\xce\xba""a\xd5\xb9\xc4\xc6\xfb\xfeX\xff\xf8\xf7\x0eY\x8d\xe9\xe1k\xcaJ.\x8aJ\x82\xebu"
"\xae\xfb_\xc5\x7f\x01\x12\xcf[\x9a\xfb\xa3""bi\0\0\0\0IEND\xae""B`\x82";

const char* disconnected_png = (const char*) temp_binary_data_11;

//================== icon.png ==================
static const unsigned char temp_binary_data_12[] =
{ 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,0,0,0,1,0,8,6,0,0,0,92,114,168,102,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,10,79,105,67,67,80,80,104,111,116,111,115,104,111,112,32,73,67,67,32,112,114,111,102,105,108,101,0,0,120,
218,157,83,103,84,83,233,22,61,247,222,244,66,75,136,128,148,75,111,82,21,8,32,82,66,139,128,20,145,38,42,33,9,16,74,136,33,161,217,21,81,193,17,69,69,4,27,200,160,136,3,142,142,128,140,21,81,44,12,138,10,216,7,228,33,162,142,131,163,136,138,202,251,
225,123,163,107,214,188,247,230,205,254,181,215,62,231,172,243,157,179,207,7,192,8,12,150,72,51,81,53,128,12,169,66,30,17,224,131,199,196,198,225,228,46,64,129,10,36,112,0,16,8,179,100,33,115,253,35,1,0,248,126,60,60,43,34,192,7,190,0,1,120,211,11,8,
0,192,77,155,192,48,28,135,255,15,234,66,153,92,1,128,132,1,192,116,145,56,75,8,128,20,0,64,122,142,66,166,0,64,70,1,128,157,152,38,83,0,160,4,0,96,203,99,98,227,0,80,45,0,96,39,127,230,211,0,128,157,248,153,123,1,0,91,148,33,21,1,160,145,0,32,19,101,
136,68,0,104,59,0,172,207,86,138,69,0,88,48,0,20,102,75,196,57,0,216,45,0,48,73,87,102,72,0,176,183,0,192,206,16,11,178,0,8,12,0,48,81,136,133,41,0,4,123,0,96,200,35,35,120,0,132,153,0,20,70,242,87,60,241,43,174,16,231,42,0,0,120,153,178,60,185,36,57,
69,129,91,8,45,113,7,87,87,46,30,40,206,73,23,43,20,54,97,2,97,154,64,46,194,121,153,25,50,129,52,15,224,243,204,0,0,160,145,21,17,224,131,243,253,120,206,14,174,206,206,54,142,182,14,95,45,234,191,6,255,34,98,98,227,254,229,207,171,112,64,0,0,225,116,
126,209,254,44,47,179,26,128,59,6,128,109,254,162,37,238,4,104,94,11,160,117,247,139,102,178,15,64,181,0,160,233,218,87,243,112,248,126,60,60,69,161,144,185,217,217,229,228,228,216,74,196,66,91,97,202,87,125,254,103,194,95,192,87,253,108,249,126,60,252,
247,245,224,190,226,36,129,50,93,129,71,4,248,224,194,204,244,76,165,28,207,146,9,132,98,220,230,143,71,252,183,11,255,252,29,211,34,196,73,98,185,88,42,20,227,81,18,113,142,68,154,140,243,50,165,34,137,66,146,41,197,37,210,255,100,226,223,44,251,3,62,
223,53,0,176,106,62,1,123,145,45,168,93,99,3,246,75,39,16,88,116,192,226,247,0,0,242,187,111,193,212,40,8,3,128,104,131,225,207,119,255,239,63,253,71,160,37,0,128,102,73,146,113,0,0,94,68,36,46,84,202,179,63,199,8,0,0,68,160,129,42,176,65,27,244,193,
24,44,192,6,28,193,5,220,193,11,252,96,54,132,66,36,196,194,66,16,66,10,100,128,28,114,96,41,172,130,66,40,134,205,176,29,42,96,47,212,64,29,52,192,81,104,134,147,112,14,46,194,85,184,14,61,112,15,250,97,8,158,193,40,188,129,9,4,65,200,8,19,97,33,218,
136,1,98,138,88,35,142,8,23,153,133,248,33,193,72,4,18,139,36,32,201,136,20,81,34,75,145,53,72,49,82,138,84,32,85,72,29,242,61,114,2,57,135,92,70,186,145,59,200,0,50,130,252,134,188,71,49,148,129,178,81,61,212,12,181,67,185,168,55,26,132,70,162,11,208,
100,116,49,154,143,22,160,155,208,114,180,26,61,140,54,161,231,208,171,104,15,218,143,62,67,199,48,192,232,24,7,51,196,108,48,46,198,195,66,177,56,44,9,147,99,203,177,34,172,12,171,198,26,176,86,172,3,187,137,245,99,207,177,119,4,18,129,69,192,9,54,4,
119,66,32,97,30,65,72,88,76,88,78,216,72,168,32,28,36,52,17,218,9,55,9,3,132,81,194,39,34,147,168,75,180,38,186,17,249,196,24,98,50,49,135,88,72,44,35,214,18,143,19,47,16,123,136,67,196,55,36,18,137,67,50,39,185,144,2,73,177,164,84,210,18,210,70,210,
110,82,35,233,44,169,155,52,72,26,35,147,201,218,100,107,178,7,57,148,44,32,43,200,133,228,157,228,195,228,51,228,27,228,33,242,91,10,157,98,64,113,164,248,83,226,40,82,202,106,74,25,229,16,229,52,229,6,101,152,50,65,85,163,154,82,221,168,161,84,17,53,
143,90,66,173,161,182,82,175,81,135,168,19,52,117,154,57,205,131,22,73,75,165,173,162,149,211,26,104,23,104,247,105,175,232,116,186,17,221,149,30,78,151,208,87,210,203,233,71,232,151,232,3,244,119,12,13,134,21,131,199,136,103,40,25,155,24,7,24,103,25,
119,24,175,152,76,166,25,211,139,25,199,84,48,55,49,235,152,231,153,15,153,111,85,88,42,182,42,124,21,145,202,10,149,74,149,38,149,27,42,47,84,169,170,166,170,222,170,11,85,243,85,203,84,143,169,94,83,125,174,70,85,51,83,227,169,9,212,150,171,85,170,
157,80,235,83,27,83,103,169,59,168,135,170,103,168,111,84,63,164,126,89,253,137,6,89,195,76,195,79,67,164,81,160,177,95,227,188,198,32,11,99,25,179,120,44,33,107,13,171,134,117,129,53,196,38,177,205,217,124,118,42,187,152,253,29,187,139,61,170,169,161,
57,67,51,74,51,87,179,82,243,148,102,63,7,227,152,113,248,156,116,78,9,231,40,167,151,243,126,138,222,20,239,41,226,41,27,166,52,76,185,49,101,92,107,170,150,151,150,88,171,72,171,81,171,71,235,189,54,174,237,167,157,166,189,69,187,89,251,129,14,65,199,
74,39,92,39,71,103,143,206,5,157,231,83,217,83,221,167,10,167,22,77,61,58,245,174,46,170,107,165,27,161,187,68,119,191,110,167,238,152,158,190,94,128,158,76,111,167,222,121,189,231,250,28,125,47,253,84,253,109,250,167,245,71,12,88,6,179,12,36,6,219,12,
206,24,60,197,53,113,111,60,29,47,199,219,241,81,67,93,195,64,67,165,97,149,97,151,225,132,145,185,209,60,163,213,70,141,70,15,140,105,198,92,227,36,227,109,198,109,198,163,38,6,38,33,38,75,77,234,77,238,154,82,77,185,166,41,166,59,76,59,76,199,205,204,
205,162,205,214,153,53,155,61,49,215,50,231,155,231,155,215,155,223,183,96,90,120,90,44,182,168,182,184,101,73,178,228,90,166,89,238,182,188,110,133,90,57,89,165,88,85,90,93,179,70,173,157,173,37,214,187,173,187,167,17,167,185,78,147,78,171,158,214,103,
195,176,241,182,201,182,169,183,25,176,229,216,6,219,174,182,109,182,125,97,103,98,23,103,183,197,174,195,238,147,189,147,125,186,125,141,253,61,7,13,135,217,14,171,29,90,29,126,115,180,114,20,58,86,58,222,154,206,156,238,63,125,197,244,150,233,47,103,
88,207,16,207,216,51,227,182,19,203,41,196,105,157,83,155,211,71,103,23,103,185,115,131,243,136,139,137,75,130,203,46,151,62,46,155,27,198,221,200,189,228,74,116,245,113,93,225,122,210,245,157,155,179,155,194,237,168,219,175,238,54,238,105,238,135,220,
159,204,52,159,41,158,89,51,115,208,195,200,67,224,81,229,209,63,11,159,149,48,107,223,172,126,79,67,79,129,103,181,231,35,47,99,47,145,87,173,215,176,183,165,119,170,247,97,239,23,62,246,62,114,159,227,62,227,60,55,222,50,222,89,95,204,55,192,183,200,
183,203,79,195,111,158,95,133,223,67,127,35,255,100,255,122,255,209,0,167,128,37,1,103,3,137,129,65,129,91,2,251,248,122,124,33,191,142,63,58,219,101,246,178,217,237,65,140,160,185,65,21,65,143,130,173,130,229,193,173,33,104,200,236,144,173,33,247,231,
152,206,145,206,105,14,133,80,126,232,214,208,7,97,230,97,139,195,126,12,39,133,135,133,87,134,63,142,112,136,88,26,209,49,151,53,119,209,220,67,115,223,68,250,68,150,68,222,155,103,49,79,57,175,45,74,53,42,62,170,46,106,60,218,55,186,52,186,63,198,46,
102,89,204,213,88,157,88,73,108,75,28,57,46,42,174,54,110,108,190,223,252,237,243,135,226,157,226,11,227,123,23,152,47,200,93,112,121,161,206,194,244,133,167,22,169,46,18,44,58,150,64,76,136,78,56,148,240,65,16,42,168,22,140,37,242,19,119,37,142,10,121,
194,29,194,103,34,47,209,54,209,136,216,67,92,42,30,78,242,72,42,77,122,146,236,145,188,53,121,36,197,51,165,44,229,185,132,39,169,144,188,76,13,76,221,155,58,158,22,154,118,32,109,50,61,58,189,49,131,146,145,144,113,66,170,33,77,147,182,103,234,103,
230,102,118,203,172,101,133,178,254,197,110,139,183,47,30,149,7,201,107,179,144,172,5,89,45,10,182,66,166,232,84,90,40,215,42,7,178,103,101,87,102,191,205,137,202,57,150,171,158,43,205,237,204,179,202,219,144,55,156,239,159,255,237,18,194,18,225,146,
182,165,134,75,87,45,29,88,230,189,172,106,57,178,60,113,121,219,10,227,21,5,43,134,86,6,172,60,184,138,182,42,109,213,79,171,237,87,151,174,126,189,38,122,77,107,129,94,193,202,130,193,181,1,107,235,11,85,10,229,133,125,235,220,215,237,93,79,88,47,89,
223,181,97,250,134,157,27,62,21,137,138,174,20,219,23,151,21,127,216,40,220,120,229,27,135,111,202,191,153,220,148,180,169,171,196,185,100,207,102,210,102,233,230,222,45,158,91,14,150,170,151,230,151,14,110,13,217,218,180,13,223,86,180,237,245,246,69,
219,47,151,205,40,219,187,131,182,67,185,163,191,60,184,188,101,167,201,206,205,59,63,84,164,84,244,84,250,84,54,238,210,221,181,97,215,248,110,209,238,27,123,188,246,52,236,213,219,91,188,247,253,62,201,190,219,85,1,85,77,213,102,213,101,251,73,251,
179,247,63,174,137,170,233,248,150,251,109,93,173,78,109,113,237,199,3,210,3,253,7,35,14,182,215,185,212,213,29,210,61,84,82,143,214,43,235,71,14,199,31,190,254,157,239,119,45,13,54,13,85,141,156,198,226,35,112,68,121,228,233,247,9,223,247,30,13,58,218,
118,140,123,172,225,7,211,31,118,29,103,29,47,106,66,154,242,154,70,155,83,154,251,91,98,91,186,79,204,62,209,214,234,222,122,252,71,219,31,15,156,52,60,89,121,74,243,84,201,105,218,233,130,211,147,103,242,207,140,157,149,157,125,126,46,249,220,96,219,
162,182,123,231,99,206,223,106,15,111,239,186,16,116,225,210,69,255,139,231,59,188,59,206,92,242,184,116,242,178,219,229,19,87,184,87,154,175,58,95,109,234,116,234,60,254,147,211,79,199,187,156,187,154,174,185,92,107,185,238,122,189,181,123,102,247,233,
27,158,55,206,221,244,189,121,241,22,255,214,213,158,57,61,221,189,243,122,111,247,197,247,245,223,22,221,126,114,39,253,206,203,187,217,119,39,238,173,188,79,188,95,244,64,237,65,217,67,221,135,213,63,91,254,220,216,239,220,127,106,192,119,160,243,209,
220,71,247,6,133,131,207,254,145,245,143,15,67,5,143,153,143,203,134,13,134,235,158,56,62,57,57,226,63,114,253,233,252,167,67,207,100,207,38,158,23,254,162,254,203,174,23,22,47,126,248,213,235,215,206,209,152,209,161,151,242,151,147,191,109,124,165,253,
234,192,235,25,175,219,198,194,198,30,190,201,120,51,49,94,244,86,251,237,193,119,220,119,29,239,163,223,15,79,228,124,32,127,40,255,104,249,177,245,83,208,167,251,147,25,147,147,255,4,3,152,243,252,99,51,45,219,0,0,58,50,105,84,88,116,88,77,76,58,99,
111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,10,60,120,58,
120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,48,54,55,32,55,57,46,49,53,55,55,
52,55,44,32,50,48,49,53,47,48,51,47,51,48,45,50,51,58,52,48,58,52,50,32,32,32,32,32,32,32,32,34,62,10,32,32,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,
57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,115,35,34,62,10,32,32,32,32,32,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,10,32,32,32,32,32,32,32,32,32,32,32,
32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,
47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,
97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,99,47,101,108,
101,109,101,110,116,115,47,49,46,49,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,112,104,111,116,111,115,104,111,112,
47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,109,108,110,115,58,116,105,102,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,116,105,102,102,47,49,46,48,47,34,10,32,32,32,32,32,32,32,32,32,32,32,32,120,
109,108,110,115,58,101,120,105,102,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,101,120,105,102,47,49,46,48,47,34,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,65,100,111,
98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,50,48,49,53,32,40,87,105,110,100,111,119,115,41,60,47,120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,67,114,101,97,116,101,68,97,116,
101,62,50,48,49,54,45,49,48,45,50,56,84,49,55,58,53,55,58,52,54,43,48,50,58,48,48,60,47,120,109,112,58,67,114,101,97,116,101,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,101,116,97,100,97,116,97,68,97,116,101,62,50,48,49,54,45,
49,48,45,50,56,84,49,55,58,53,55,58,52,54,43,48,50,58,48,48,60,47,120,109,112,58,77,101,116,97,100,97,116,97,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,50,48,49,54,45,49,48,45,50,56,84,49,
55,58,53,55,58,52,54,43,48,50,58,48,48,60,47,120,109,112,58,77,111,100,105,102,121,68,97,116,101,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,62,120,109,112,46,105,105,100,58,48,52,98,50,100,102,49,53,45,
102,101,56,99,45,98,53,52,49,45,98,98,52,98,45,51,57,57,49,97,102,50,55,99,98,54,99,60,47,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,62,97,100,111,
98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,49,56,50,97,57,48,54,97,45,57,100,50,55,45,49,49,101,54,45,97,54,56,48,45,56,51,49,102,102,99,98,48,54,55,55,100,60,47,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,62,
10,32,32,32,32,32,32,32,32,32,60,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,62,120,109,112,46,100,105,100,58,52,99,49,51,53,50,101,49,45,49,101,98,55,45,54,56,52,100,45,98,98,49,55,45,101,97,55,53,51,57,57,
56,97,49,48,56,60,47,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,62,10,32,32,32,32,32,32,32,32,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,114,100,102,58,
83,101,113,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,114,100,102,58,108,105,32,114,100,102,58,112,97,114,115,101,84,121,112,101,61,34,82,101,115,111,117,114,99,101,34,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,
69,118,116,58,97,99,116,105,111,110,62,99,114,101,97,116,101,100,60,47,115,116,69,118,116,58,97,99,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,62,120,109,112,
46,105,105,100,58,52,99,49,51,53,50,101,49,45,49,101,98,55,45,54,56,52,100,45,98,98,49,55,45,101,97,55,53,51,57,57,56,97,49,48,56,60,47,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,60,115,116,69,118,116,58,119,104,101,110,62,50,48,49,54,45,49,48,45,50,56,84,49,55,58,53,55,58,52,54,43,48,50,58,48,48,60,47,115,116,69,118,116,58,119,104,101,110,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,69,118,116,58,
115,111,102,116,119,97,114,101,65,103,101,110,116,62,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,50,48,49,53,32,40,87,105,110,100,111,119,115,41,60,47,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,62,
10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,47,114,100,102,58,108,105,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,114,100,102,58,108,105,32,114,100,102,58,112,97,114,115,101,84,121,112,101,61,34,82,101,115,111,117,114,99,101,34,62,10,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,69,118,116,58,97,99,116,105,111,110,62,115,97,118,101,100,60,47,115,116,69,118,116,58,97,99,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,69,118,
116,58,105,110,115,116,97,110,99,101,73,68,62,120,109,112,46,105,105,100,58,48,52,98,50,100,102,49,53,45,102,101,56,99,45,98,53,52,49,45,98,98,52,98,45,51,57,57,49,97,102,50,55,99,98,54,99,60,47,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,
68,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,69,118,116,58,119,104,101,110,62,50,48,49,54,45,49,48,45,50,56,84,49,55,58,53,55,58,52,54,43,48,50,58,48,48,60,47,115,116,69,118,116,58,119,104,101,110,62,10,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,62,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,50,48,49,53,32,40,87,105,110,100,111,119,115,41,60,47,115,116,69,118,
116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,60,115,116,69,118,116,58,99,104,97,110,103,101,100,62,47,60,47,115,116,69,118,116,58,99,104,97,110,103,101,100,62,10,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,60,47,114,100,102,58,108,105,62,10,32,32,32,32,32,32,32,32,32,32,32,32,60,47,114,100,102,58,83,101,113,62,10,32,32,32,32,32,32,32,32,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,10,32,32,32,32,32,32,32,32,32,
60,100,99,58,102,111,114,109,97,116,62,105,109,97,103,101,47,112,110,103,60,47,100,99,58,102,111,114,109,97,116,62,10,32,32,32,32,32,32,32,32,32,60,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,62,51,60,47,112,104,111,116,111,
115,104,111,112,58,67,111,108,111,114,77,111,100,101,62,10,32,32,32,32,32,32,32,32,32,60,112,104,111,116,111,115,104,111,112,58,73,67,67,80,114,111,102,105,108,101,62,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,60,47,112,104,111,116,111,115,104,
111,112,58,73,67,67,80,114,111,102,105,108,101,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,49,60,47,116,105,102,102,58,79,114,105,101,110,116,97,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,
60,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,55,50,48,48,48,48,47,49,48,48,48,48,60,47,116,105,102,102,58,88,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,89,82,101,115,111,108,117,
116,105,111,110,62,55,50,48,48,48,48,47,49,48,48,48,48,60,47,116,105,102,102,58,89,82,101,115,111,108,117,116,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,116,105,102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,50,60,47,116,105,
102,102,58,82,101,115,111,108,117,116,105,111,110,85,110,105,116,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,49,60,47,101,120,105,102,58,67,111,108,111,114,83,112,97,99,101,62,10,32,32,32,32,32,32,32,
32,32,60,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,50,53,54,60,47,101,120,105,102,58,80,105,120,101,108,88,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,32,32,32,60,101,120,105,102,58,80,105,120,101,108,
89,68,105,109,101,110,115,105,111,110,62,50,53,54,60,47,101,120,105,102,58,80,105,120,101,108,89,68,105,109,101,110,115,105,111,110,62,10,32,32,32,32,32,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,10,32,32,32,60,47,114,100,102,
58,82,68,70,62,10,60,47,120,58,120,109,112,109,101,116,97,62,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,60,63,120,112,97,99,107,101,116,32,101,110,100,61,34,119,34,63,62,202,220,244,
112,0,0,0,32,99,72,82,77,0,0,122,37,0,0,128,131,0,0,249,255,0,0,128,233,0,0,117,48,0,0,234,96,0,0,58,152,0,0,23,111,146,95,197,70,0,0,95,150,73,68,65,84,120,218,236,157,119,156,93,69,249,135,159,57,231,220,115,251,246,150,108,122,111,132,0,33,148,208,
66,51,116,164,43,138,136,160,32,2,162,168,40,250,19,11,138,138,216,21,196,6,162,72,17,233,189,215,80,18,32,16,210,123,217,94,111,63,109,126,127,204,221,52,18,216,36,155,144,50,207,231,115,41,201,238,221,187,231,156,249,206,204,59,239,251,125,133,148,
18,141,70,179,103,98,232,75,160,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,
22,0,141,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,
70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,
11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,217,94,88,189,249,34,33,132,190,82,187,54,2,136,
2,37,64,28,176,129,10,64,22,95,0,38,208,1,228,128,44,208,9,228,245,165,219,181,145,82,110,187,0,104,118,41,108,96,16,48,26,40,5,106,128,126,192,64,96,108,241,239,194,64,108,19,34,145,45,14,250,21,192,108,96,1,176,10,104,44,254,127,131,190,188,187,217,
204,240,81,10,161,87,0,59,61,21,192,8,96,255,226,0,175,1,198,1,227,123,190,160,42,106,80,22,49,169,136,152,148,218,130,144,33,72,218,6,166,33,8,138,247,223,50,4,221,133,128,156,23,208,89,8,104,205,5,52,100,60,242,222,218,231,227,21,224,93,96,6,240,28,
176,76,95,250,93,127,5,160,5,96,215,100,52,112,2,48,169,56,232,251,1,245,66,8,106,98,6,123,85,133,25,94,22,98,68,121,136,186,132,201,192,132,73,117,212,162,52,110,17,143,24,152,66,18,17,2,33,214,109,0,132,128,66,0,142,132,116,206,167,61,237,177,34,229,
177,162,219,227,221,22,135,25,13,57,222,111,115,113,124,41,129,185,192,107,192,239,129,89,250,118,104,1,208,108,95,34,197,37,252,73,192,103,138,3,190,198,50,48,6,38,45,198,84,134,153,92,103,115,72,255,40,35,202,66,148,70,12,146,182,129,29,54,138,187,
124,185,118,183,239,3,70,113,224,111,234,182,74,64,200,226,134,192,80,95,224,184,1,157,57,159,249,29,30,15,44,202,240,248,210,12,239,181,58,72,104,1,30,7,126,8,44,212,183,73,11,128,166,239,72,2,35,129,131,129,243,129,137,2,66,85,81,147,33,165,22,199,
14,141,49,165,95,148,73,213,33,250,151,135,48,133,0,55,192,11,212,32,14,2,73,0,4,82,141,99,203,20,216,166,186,143,5,79,146,246,2,10,62,4,197,31,102,0,81,75,144,176,4,33,83,224,7,18,199,87,239,99,26,96,8,65,200,0,25,50,232,202,248,220,183,40,205,63,223,
77,241,242,154,60,142,47,93,224,7,192,205,64,171,190,117,90,0,52,91,207,222,192,100,224,100,96,58,96,215,198,76,38,84,219,76,27,24,229,184,225,113,70,148,135,40,137,89,32,192,47,4,228,221,128,160,56,105,111,120,243,33,98,27,132,194,2,215,149,204,107,
46,240,202,234,2,111,54,22,152,223,225,176,38,237,81,240,36,8,136,152,6,131,74,76,38,84,133,153,54,40,202,97,131,163,84,36,45,220,92,64,206,13,122,22,3,72,9,97,91,96,71,76,220,92,192,223,231,116,243,219,55,58,153,211,230,0,188,14,92,86,252,183,70,11,
128,102,11,152,14,28,7,156,1,244,47,181,13,166,212,133,57,110,120,156,41,253,35,236,95,27,198,182,85,218,134,239,7,20,28,73,113,236,110,122,41,47,33,26,50,8,4,60,176,48,195,221,243,82,188,214,144,103,121,183,215,171,15,115,212,160,40,159,159,80,194,217,
19,146,16,64,214,217,80,4,4,16,14,27,132,66,6,75,59,28,174,127,165,157,191,188,211,77,0,109,69,17,184,67,223,82,45,0,154,143,230,28,224,243,192,1,64,233,196,106,155,147,134,39,56,118,88,140,189,250,133,41,15,27,16,128,235,4,184,129,196,15,122,238,201,
135,221,116,53,75,167,28,201,87,158,104,230,254,69,89,114,94,176,197,31,204,50,224,172,49,9,126,117,84,53,21,97,147,156,19,108,240,115,101,113,123,17,15,27,248,166,224,159,239,116,115,245,115,173,52,101,253,28,240,19,224,199,250,246,106,1,208,124,144,
48,112,46,112,49,48,222,20,196,14,25,16,225,130,9,165,28,53,56,74,255,10,27,17,72,92,95,226,184,1,65,113,125,223,155,187,32,1,83,64,44,110,241,203,87,218,185,234,217,109,223,146,127,231,192,114,174,155,86,69,62,239,227,6,155,222,106,132,67,2,59,110,242,
228,188,52,23,60,218,194,170,148,27,0,223,43,10,129,102,39,21,0,157,8,180,99,169,0,142,0,126,4,140,77,134,12,113,216,192,40,23,236,93,194,244,33,81,98,97,3,233,67,46,231,169,65,207,71,207,246,155,188,169,166,0,63,96,94,171,243,161,95,23,50,4,81,75,128,
132,156,47,113,131,77,63,44,175,174,206,227,57,1,150,41,112,125,249,1,5,16,66,5,22,189,110,143,99,70,37,184,51,98,112,206,255,26,141,149,41,239,123,168,164,162,219,245,173,223,57,209,2,176,99,168,45,238,241,47,4,14,169,142,153,28,54,32,202,121,123,37,
57,126,88,12,203,50,200,229,124,82,185,128,109,93,107,9,192,245,36,118,216,224,200,33,81,110,157,211,141,187,158,152,84,68,12,70,150,217,28,92,31,102,84,101,152,65,229,33,0,86,180,187,60,191,34,203,203,107,242,172,76,109,24,39,216,183,95,24,43,100,80,
40,4,155,93,134,8,192,151,144,73,121,28,60,40,198,159,166,215,240,217,7,26,35,29,133,224,87,168,108,194,231,244,99,176,243,161,183,0,219,159,243,129,47,0,135,36,108,131,51,70,39,248,252,196,18,14,27,24,1,211,160,144,247,41,56,18,163,15,203,178,164,132,
144,169,142,243,126,241,122,7,183,205,73,225,7,146,131,235,35,124,102,124,146,41,253,163,36,195,134,218,192,155,197,111,242,129,32,96,86,147,203,31,223,232,228,254,197,105,178,110,192,39,134,198,249,205,209,85,212,37,45,242,133,143,22,40,41,213,145,98,
188,212,226,214,153,93,92,248,88,19,94,192,66,224,40,96,165,126,28,116,12,96,79,225,4,224,74,224,40,219,20,156,56,44,198,87,38,151,113,196,160,24,66,168,228,154,130,43,145,172,205,183,233,227,27,15,182,37,176,13,193,146,110,15,31,201,176,132,137,21,54,
241,61,137,231,75,124,95,226,23,111,191,105,20,115,5,138,51,253,156,78,23,39,8,24,91,106,83,18,50,72,123,189,95,157,244,8,80,196,54,248,234,147,205,252,102,86,23,192,159,138,215,163,160,31,13,45,0,91,179,178,149,187,200,53,31,9,124,31,56,73,8,74,166,
15,141,243,245,253,203,56,96,112,148,132,48,112,188,0,19,137,105,138,181,35,223,119,3,178,174,164,175,47,179,4,66,6,132,45,181,188,112,124,21,88,148,197,76,191,77,5,243,4,16,178,4,97,75,125,129,235,73,149,43,176,21,63,59,25,54,104,204,7,156,113,79,3,
47,175,201,185,192,137,192,19,122,88,106,1,216,28,17,84,154,107,57,208,191,24,52,27,6,212,21,103,14,11,149,114,218,132,170,76,107,69,165,163,54,239,4,2,81,85,220,227,127,7,72,238,93,99,115,213,148,114,78,31,149,32,26,49,193,11,192,16,72,3,90,186,61,230,
181,58,180,100,125,66,166,96,74,191,48,117,113,139,244,135,236,177,119,201,253,165,128,120,194,226,238,247,186,249,244,3,77,120,129,124,3,152,6,100,244,208,212,2,208,67,37,176,95,113,230,60,0,149,239,94,6,235,162,212,17,75,172,157,181,178,110,64,214,
43,206,98,138,119,128,71,129,247,128,55,129,249,31,195,117,62,14,149,10,187,255,224,18,139,47,238,93,202,23,246,46,161,54,97,33,189,0,97,10,114,62,204,92,147,231,145,69,25,94,89,147,231,181,198,252,218,74,187,19,134,197,185,101,122,53,53,49,139,156,27,
236,70,15,31,68,108,129,99,8,206,189,183,129,251,23,101,0,206,3,254,169,135,166,22,128,125,138,251,228,105,192,145,0,53,49,147,49,21,54,195,74,45,134,148,134,168,79,90,212,68,77,202,99,38,65,160,102,148,166,140,75,99,38,96,101,177,82,109,126,187,195,
251,109,5,28,31,138,171,131,23,80,17,231,59,138,161,173,237,189,220,255,38,112,97,216,18,92,48,190,132,47,237,95,198,222,117,97,188,156,15,134,192,50,224,241,197,89,110,122,187,139,103,87,230,232,42,108,122,128,255,114,90,37,95,59,176,130,108,206,39,
144,236,86,34,144,76,154,60,178,32,195,25,255,107,32,231,201,185,192,4,214,149,33,104,62,70,1,248,56,142,1,71,0,215,2,135,0,131,43,163,6,135,214,71,57,110,88,140,201,181,17,234,75,76,106,147,33,21,74,222,228,103,143,170,63,55,192,115,37,171,58,93,22,
119,185,60,183,50,199,227,75,179,35,223,104,42,140,68,242,121,224,34,224,22,182,223,25,244,197,192,87,129,209,147,107,195,92,115,112,5,167,140,78,32,12,200,166,61,162,17,131,214,108,192,53,207,183,113,215,188,212,102,7,126,15,173,217,96,109,176,99,183,
66,64,33,23,112,204,144,40,71,13,142,241,208,226,204,40,224,20,224,127,122,120,238,4,183,103,7,174,0,172,226,192,191,0,232,55,182,50,204,167,198,198,57,117,100,156,33,85,54,73,161,18,82,2,41,113,131,98,5,171,84,21,109,235,127,68,67,244,228,191,11,12,
161,130,92,194,16,72,160,205,145,188,178,60,195,63,223,79,243,224,162,12,5,95,230,80,6,22,87,209,119,117,235,117,192,207,129,115,162,33,17,250,250,254,101,92,177,95,57,85,49,147,130,27,224,122,146,68,204,228,221,198,2,23,62,222,196,235,13,31,29,244,174,
142,154,220,126,98,29,199,14,139,146,206,239,250,19,163,40,158,46,250,114,93,53,114,50,110,242,208,252,12,103,220,223,64,193,151,207,163,18,162,52,123,200,22,96,32,170,84,244,184,138,136,193,55,166,148,115,193,222,165,84,151,89,4,133,0,199,145,4,108,
176,175,223,98,148,24,8,66,17,131,192,151,60,191,60,207,141,111,116,240,196,210,44,78,32,11,192,117,192,111,128,238,109,248,61,142,7,254,0,12,57,116,64,132,31,30,90,197,225,67,163,224,72,210,110,128,144,144,72,90,188,182,60,203,103,31,110,102,97,199,
230,51,241,108,67,80,25,53,25,144,52,249,206,129,21,156,50,42,65,222,13,240,2,185,203,175,2,226,182,32,64,96,10,72,23,2,85,152,100,27,100,92,201,180,59,86,241,86,115,161,161,24,55,121,71,15,209,221,95,0,198,1,127,1,14,58,118,112,140,239,31,90,193,193,
131,98,248,110,64,174,16,32,69,223,46,123,3,32,98,10,236,176,129,235,6,252,125,78,138,27,94,237,96,97,167,11,240,8,240,109,148,191,221,150,16,3,46,1,110,136,89,130,11,38,150,240,131,67,42,169,72,88,100,178,42,109,87,2,37,81,131,37,237,46,159,188,175,
145,217,45,155,158,249,43,34,6,211,6,70,249,196,240,56,211,6,68,25,92,18,34,100,65,206,145,106,240,239,226,163,63,30,50,120,114,89,150,71,151,102,57,99,116,156,3,234,163,228,29,181,170,73,36,45,126,248,92,43,223,127,169,29,224,167,168,19,19,205,110,44,
0,99,128,123,128,241,95,222,183,148,235,167,85,145,12,27,100,178,254,218,160,222,118,249,165,81,174,54,225,144,32,20,179,88,208,92,224,7,207,183,242,239,121,105,80,217,104,95,43,126,174,222,6,250,174,7,78,27,83,97,243,195,67,43,56,115,92,9,158,23,144,
47,30,219,169,98,24,131,156,12,56,253,158,6,158,94,145,219,228,27,157,56,44,198,87,15,170,224,168,1,17,85,106,231,4,184,190,164,224,239,30,81,63,219,84,179,254,169,255,107,224,161,37,89,142,27,30,227,190,79,214,33,2,65,206,147,148,196,77,94,89,158,229,
152,187,215,144,117,229,2,224,6,192,69,21,71,165,80,117,3,61,47,205,46,30,4,140,3,55,1,227,47,217,167,148,95,125,162,134,80,0,169,148,15,6,219,117,166,19,197,127,228,93,73,190,203,101,84,121,136,191,157,90,199,164,25,29,252,240,229,142,129,105,55,184,
29,117,252,120,243,71,188,213,52,84,6,219,232,79,12,141,241,187,163,107,24,89,107,147,205,248,120,158,74,223,149,128,97,128,29,49,248,205,75,29,155,28,252,2,184,230,160,10,174,58,184,140,210,136,69,46,231,227,229,188,181,127,185,59,4,254,36,202,88,180,
35,239,211,150,87,135,47,73,75,96,4,66,153,149,8,200,231,125,246,234,31,97,239,234,48,175,174,201,143,2,254,188,222,91,120,40,47,129,22,84,126,199,92,224,89,148,229,88,183,30,202,219,135,237,41,0,215,1,135,159,56,44,198,141,211,170,176,125,73,119,62,
232,211,156,247,222,4,163,36,144,202,250,196,194,6,223,152,90,73,255,132,197,87,159,110,13,183,230,252,223,22,159,219,63,111,230,219,79,3,254,34,160,252,43,251,148,114,237,17,85,84,68,12,50,105,181,228,95,251,123,72,136,69,76,22,181,59,252,253,221,212,
38,223,232,167,135,87,242,141,131,43,49,252,128,116,202,83,43,148,221,45,220,47,193,48,5,141,133,128,230,140,18,128,49,149,97,172,136,73,58,227,33,80,185,80,97,75,240,157,3,203,153,213,92,160,46,102,129,132,180,235,211,89,8,172,5,157,110,237,242,46,175,
118,89,151,75,115,214,63,34,144,156,87,20,132,215,128,95,0,111,179,253,143,118,181,0,244,1,7,1,103,245,79,152,92,63,173,154,72,200,32,149,243,183,75,206,123,111,87,3,57,39,192,112,3,206,157,88,74,93,220,228,179,15,53,219,13,25,239,38,148,23,254,250,71,
133,33,224,82,224,23,101,97,195,186,254,136,42,190,184,111,41,190,19,144,206,250,107,133,101,237,5,52,64,24,240,208,188,12,115,219,63,24,244,187,124,223,82,174,154,90,129,239,248,100,93,185,219,204,248,155,18,91,12,88,210,225,176,166,40,0,227,42,109,
48,138,41,198,197,95,218,41,4,28,55,44,198,9,35,226,235,204,71,1,41,192,55,5,110,33,96,105,187,203,236,182,2,207,46,207,197,31,91,150,139,55,165,189,33,5,95,158,142,202,241,248,37,240,146,94,21,236,188,2,96,0,103,3,253,174,218,191,156,241,253,194,100,
82,222,78,49,227,5,18,178,89,159,163,70,39,248,139,7,167,223,223,32,242,158,252,39,235,202,85,109,84,70,223,213,253,227,38,127,58,182,150,147,199,39,200,103,125,92,111,211,1,58,211,20,56,1,204,88,243,193,165,255,152,242,16,95,59,160,28,195,151,42,215,
127,7,47,201,215,183,252,222,222,132,76,129,20,240,228,226,44,57,79,210,63,110,50,165,95,152,96,227,252,7,1,57,119,253,134,68,27,111,35,96,108,117,152,113,245,17,206,25,155,100,85,202,227,169,165,57,238,154,155,178,94,92,157,63,50,237,6,71,2,15,21,133,
224,57,61,132,119,62,1,24,12,28,55,172,52,196,105,227,146,248,133,128,64,178,93,10,93,54,120,134,122,57,179,250,129,36,159,242,57,126,92,130,159,116,84,242,181,103,90,65,249,219,127,18,248,20,112,245,184,10,155,155,167,215,112,200,208,24,217,180,135,
255,17,1,75,67,192,166,226,120,159,28,149,96,112,69,136,108,198,223,161,131,63,144,16,177,148,67,15,133,64,109,189,182,243,7,8,219,130,133,109,14,15,46,86,105,254,83,7,68,25,88,97,147,207,249,189,190,247,2,240,3,72,231,125,100,94,137,74,125,210,226,252,
125,74,248,236,184,36,143,46,207,242,247,119,186,248,223,162,204,137,82,114,36,240,119,84,110,201,206,230,68,28,6,170,89,215,133,41,1,12,7,218,139,147,77,1,88,83,124,21,118,55,1,152,4,140,58,97,88,140,129,113,139,130,227,247,249,224,23,64,216,20,152,
235,61,213,190,47,113,138,86,216,226,35,150,170,174,47,17,105,159,175,78,41,103,89,151,199,111,103,118,142,5,30,4,134,236,91,27,230,214,147,235,152,80,19,86,251,253,143,152,65,61,95,18,179,37,167,143,78,240,216,210,44,233,98,46,255,248,42,155,47,76,44,
65,186,170,228,118,71,10,64,212,18,180,228,124,254,55,39,197,126,213,54,83,250,71,200,22,182,95,130,145,41,212,185,255,223,102,117,179,172,219,195,50,224,243,227,147,88,129,220,170,230,130,162,40,230,94,32,73,231,212,202,43,108,9,78,28,21,231,200,97,
49,30,157,151,230,123,47,183,199,230,182,57,151,162,234,71,174,64,117,46,250,184,232,143,234,204,52,185,248,74,162,250,48,86,21,183,148,145,162,8,4,168,254,139,110,241,223,237,64,23,235,186,45,189,178,163,99,28,219,67,0,134,3,28,62,48,134,33,36,125,121,
194,101,26,130,168,45,192,20,164,51,62,237,25,15,16,8,36,101,17,139,100,220,2,95,146,43,4,248,31,114,252,33,4,20,220,128,112,220,228,210,125,75,249,235,236,46,35,227,202,209,19,171,109,254,113,98,45,19,106,237,117,193,186,143,90,81,72,200,229,2,206,28,
155,160,52,36,184,111,113,150,154,152,193,167,198,38,25,94,17,34,147,15,182,219,224,55,196,186,76,187,117,131,17,66,97,131,155,103,180,243,227,87,59,24,148,12,241,228,57,245,140,168,176,232,206,244,125,16,86,74,73,52,102,242,204,146,44,191,123,187,19,
128,227,135,196,56,124,72,140,130,43,183,89,232,123,46,94,222,147,228,61,159,72,72,112,250,196,82,246,173,143,240,211,151,219,249,235,187,221,147,3,201,127,81,94,3,255,217,65,227,198,64,185,60,157,138,74,107,30,15,148,9,72,36,108,131,154,168,73,125,137,
197,144,18,139,18,219,160,60,98,80,19,179,232,44,248,70,75,206,175,204,184,146,197,157,78,221,202,46,159,198,172,71,198,149,199,23,197,96,37,112,55,240,55,118,80,31,70,107,59,188,223,192,170,152,73,125,137,217,39,5,186,107,157,103,109,3,71,8,222,108,
200,241,228,178,44,207,175,200,179,160,195,93,187,29,24,93,17,98,218,160,40,211,135,198,24,87,27,33,42,37,57,87,25,95,108,60,131,7,18,18,49,147,150,78,151,111,62,215,74,198,149,76,172,14,115,251,41,181,236,85,19,38,221,237,247,122,75,209,19,221,22,78,
192,113,195,227,28,59,52,134,33,4,8,73,58,31,244,74,68,182,102,251,99,27,130,112,72,32,3,245,123,250,108,104,154,80,159,180,8,36,44,235,118,249,242,19,205,220,115,74,29,37,113,147,212,38,2,153,91,123,95,132,128,100,34,196,242,54,135,175,63,211,66,198,
149,36,67,130,43,246,47,39,86,204,247,232,211,96,46,80,112,37,5,207,101,72,210,226,166,227,106,153,88,19,230,234,231,219,234,50,110,112,83,241,249,219,158,254,131,85,192,196,226,138,99,58,16,42,181,13,49,164,212,98,234,128,40,19,170,195,236,93,101,51,
182,210,38,17,82,15,144,129,74,89,239,185,222,126,0,18,149,249,154,41,72,222,111,119,121,97,117,142,23,86,228,42,102,54,230,43,90,114,193,222,192,255,161,142,168,127,9,44,223,174,193,219,62,78,4,138,3,255,26,91,105,159,114,239,41,117,140,169,180,73,59,
193,54,61,100,33,83,16,137,25,44,106,117,185,241,181,118,238,158,159,166,53,23,128,106,78,185,2,117,126,28,66,249,6,212,87,68,76,206,26,29,231,162,73,165,236,219,47,66,224,75,178,235,101,28,6,64,44,108,224,9,193,23,30,108,228,223,239,167,24,82,106,241,
191,51,235,153,84,107,147,74,251,27,60,112,91,251,160,110,207,212,30,219,18,52,164,61,110,125,47,197,200,178,16,159,28,149,64,74,240,165,74,167,142,132,212,42,233,75,143,54,243,247,119,85,176,252,164,225,49,254,122,82,29,213,113,139,108,218,199,151,91,
159,117,40,3,176,109,65,56,106,177,184,165,192,231,30,110,226,229,85,42,8,250,227,67,43,185,230,176,10,242,89,149,214,188,221,130,156,82,229,29,68,227,6,255,121,47,205,151,31,107,162,163,16,228,128,47,209,247,229,198,163,129,99,128,207,1,147,227,33,131,
113,149,33,14,31,24,229,216,17,113,246,171,10,83,145,52,1,129,116,3,242,174,92,219,116,117,125,209,22,235,63,36,82,173,214,194,33,3,17,54,112,243,1,51,155,243,60,48,63,195,127,23,164,123,38,183,38,84,221,201,173,168,28,137,173,90,161,237,72,1,72,2,119,
77,170,9,79,255,239,169,117,12,43,181,72,59,91,247,16,4,18,162,150,65,40,98,112,199,187,221,252,232,229,246,158,99,182,55,128,123,89,231,1,224,23,149,127,63,224,19,40,63,129,201,181,113,147,75,38,149,113,229,1,229,148,132,213,150,65,162,178,213,194,49,
147,31,60,219,202,181,175,180,211,47,97,114,199,137,117,28,62,34,78,186,123,231,63,163,23,64,60,102,242,251,215,59,185,236,233,22,134,149,134,120,252,204,254,140,168,8,145,42,122,246,73,9,209,136,65,202,11,56,251,222,70,158,92,150,5,224,232,65,81,126,
121,76,53,19,251,71,241,178,62,89,39,64,24,189,23,187,160,248,208,198,99,38,24,130,7,231,165,185,250,217,22,222,111,87,43,177,207,142,47,225,230,19,107,49,125,73,97,163,30,2,219,79,4,32,154,12,113,207,187,221,92,252,88,51,109,121,191,11,56,19,120,178,
143,6,254,231,81,57,33,35,171,163,38,211,135,197,56,99,84,130,195,7,71,41,141,90,96,130,239,168,172,80,255,67,26,181,124,216,53,5,37,218,118,68,237,207,22,180,186,220,254,78,23,55,189,211,77,75,206,7,120,6,101,177,254,202,206,46,0,54,240,167,65,37,214,
5,247,127,178,31,147,106,195,202,229,102,43,150,184,97,83,96,135,77,126,250,114,59,63,126,165,141,172,39,3,84,254,248,77,197,72,234,230,24,136,242,25,248,53,16,158,62,52,198,175,143,169,102,116,149,202,224,139,69,77,254,245,110,55,23,62,222,12,8,110,
59,190,134,51,247,74,146,73,251,31,25,64,220,105,4,32,98,112,253,140,14,190,253,124,27,195,74,67,220,247,201,126,236,85,109,147,114,214,197,27,164,132,68,196,160,49,235,115,217,19,45,252,119,65,26,128,33,37,22,95,63,160,156,47,78,42,197,14,9,156,156,
143,43,217,108,33,86,207,3,109,162,58,1,97,9,222,107,118,248,235,172,78,254,58,187,155,84,113,159,127,201,62,165,92,119,120,21,37,33,65,182,16,236,48,17,149,20,87,137,9,139,91,103,117,114,217,19,45,164,156,96,57,112,248,54,44,159,75,129,175,20,103,252,
145,131,75,44,206,24,149,224,220,189,146,236,85,21,198,50,5,129,47,113,61,137,83,12,114,245,197,150,10,32,100,66,196,54,193,128,89,107,242,252,244,213,118,238,89,144,161,24,19,248,62,170,196,189,207,4,160,175,99,0,14,176,112,85,202,99,69,151,199,164,
186,200,86,206,252,130,144,109,240,147,151,218,248,191,151,218,240,37,205,192,229,192,157,189,120,139,149,69,145,120,9,248,225,99,75,179,159,60,229,238,53,252,233,19,53,76,27,151,100,246,210,12,95,127,182,149,188,39,185,241,136,74,206,220,171,132,92,
214,219,37,6,255,250,68,138,39,32,134,161,12,61,55,21,232,76,231,3,250,197,77,254,118,114,45,67,158,15,241,219,153,157,44,235,246,184,226,169,22,254,59,63,205,103,198,37,57,105,104,140,202,184,169,78,84,12,177,174,147,112,207,200,247,85,145,82,119,193,
231,217,85,57,238,93,152,229,129,133,105,154,138,251,251,178,176,193,213,7,150,115,229,1,21,216,82,170,85,136,216,177,130,232,120,18,153,242,248,220,196,82,22,182,58,92,55,163,99,48,170,0,237,216,45,220,141,153,192,103,139,123,240,161,213,49,147,47,236,
85,194,133,19,75,24,82,101,99,6,146,130,35,201,59,193,58,95,197,62,250,93,123,222,199,245,193,205,41,171,184,125,7,70,248,215,192,122,142,122,189,131,31,190,210,222,175,33,237,255,30,101,147,247,243,190,218,101,110,143,83,128,37,129,132,167,150,103,57,
121,76,2,83,136,15,141,200,111,188,183,140,132,13,12,91,240,131,23,218,249,225,75,109,4,176,4,213,69,103,198,22,126,142,247,80,109,183,190,63,191,195,189,250,11,143,54,27,127,240,37,127,126,171,139,166,172,207,167,199,38,185,100,74,25,158,227,171,115,
126,118,45,252,245,167,64,185,249,135,42,149,11,72,132,13,126,113,100,21,7,245,143,240,195,151,219,152,221,234,240,220,138,28,207,173,200,49,184,196,226,136,129,81,246,173,139,48,48,105,82,102,27,68,67,6,41,39,32,229,4,44,238,116,121,167,165,192,27,13,
5,22,117,186,235,150,172,166,224,240,65,81,174,157,90,193,129,131,162,184,133,128,148,251,241,84,51,26,197,163,93,219,9,184,250,160,10,94,92,149,231,133,85,185,163,81,134,45,191,234,229,219,76,68,21,39,29,93,26,54,196,233,163,18,124,115,74,25,163,106,
194,136,64,146,203,7,248,235,197,52,196,118,114,114,182,44,65,212,50,192,130,116,54,160,57,229,49,169,58,204,212,254,17,238,89,144,177,81,133,105,73,84,194,154,187,179,5,1,65,29,3,62,85,159,176,134,60,113,78,61,227,106,84,96,237,163,222,162,199,63,206,
10,155,92,247,66,43,215,190,212,142,47,233,64,101,21,110,235,126,238,7,192,183,67,134,8,185,129,100,124,165,205,253,103,246,103,104,105,136,76,222,223,165,6,191,41,4,145,144,224,235,207,180,242,171,153,157,236,91,27,230,225,211,250,81,27,183,148,39,193,
230,150,201,134,32,18,49,88,211,229,113,251,251,221,252,107,78,138,121,237,238,218,37,236,250,131,169,196,54,233,44,108,58,130,63,56,105,177,79,109,152,179,199,171,213,67,60,108,146,205,251,107,79,33,62,78,164,132,68,212,228,141,213,57,62,113,207,26,
58,243,193,124,84,63,130,213,31,242,109,21,197,192,225,53,64,252,232,193,81,190,121,80,5,199,12,138,34,37,228,157,96,187,31,204,171,96,119,113,233,15,188,219,230,242,236,146,52,79,47,203,240,234,202,28,45,155,30,230,191,64,149,83,123,59,83,12,160,135,
107,128,31,31,63,36,198,237,103,244,163,44,100,144,73,251,42,18,47,62,248,112,246,20,212,152,38,92,247,82,27,215,190,212,142,39,105,45,222,152,123,251,232,58,223,12,92,24,54,132,113,239,105,117,28,63,166,132,116,202,101,87,66,162,218,120,7,72,14,191,
99,21,175,53,20,152,62,52,198,67,167,247,39,8,36,121,255,195,211,141,101,160,130,131,86,88,176,166,219,231,249,37,25,158,94,158,99,65,135,203,138,110,143,206,130,191,129,117,89,137,109,80,30,53,25,146,180,24,84,98,177,111,109,152,35,134,197,217,171,50,
132,105,27,20,50,62,5,95,126,44,53,30,155,187,62,6,16,79,90,124,251,169,22,174,159,209,65,113,223,252,195,205,124,203,81,40,127,136,163,134,148,90,124,125,114,57,159,223,183,148,184,109,144,205,250,120,219,249,119,91,219,68,37,166,202,99,103,172,206,
115,251,187,221,220,191,32,197,170,140,186,15,3,6,13,100,242,164,9,212,15,232,79,93,109,13,121,199,227,246,127,222,193,242,21,171,0,190,139,42,186,219,233,4,160,10,229,255,190,207,217,99,18,252,250,19,213,212,37,67,42,167,126,189,135,84,202,98,84,62,
110,146,201,250,252,248,229,54,110,120,189,19,79,146,65,185,199,222,219,135,215,251,167,192,183,46,152,80,34,254,122,106,29,133,172,143,227,237,90,6,28,82,66,34,108,48,187,57,207,161,119,172,38,229,72,190,185,127,25,63,59,170,74,37,63,5,189,124,232,12,
213,209,23,83,64,0,77,41,143,134,148,71,183,27,208,145,11,240,164,36,100,8,74,195,6,101,97,131,254,9,139,234,132,169,130,13,129,196,241,84,148,127,103,44,108,146,82,5,73,23,119,186,28,246,175,85,52,102,252,133,40,247,161,197,27,125,233,255,21,227,74,
149,167,143,76,240,131,195,43,25,95,167,106,23,50,133,237,255,187,201,98,245,100,60,102,176,172,213,229,87,111,118,112,215,220,52,141,89,31,51,28,229,180,147,167,115,214,233,39,49,126,175,125,25,50,120,48,209,120,217,218,239,125,233,249,39,248,212,185,
231,179,106,117,131,131,234,58,117,251,206,38,0,160,106,2,158,4,70,238,93,109,243,221,169,149,76,31,18,35,26,49,48,138,203,206,192,20,100,114,62,207,173,202,243,171,215,219,121,126,101,30,9,75,139,17,216,71,250,240,122,31,1,220,57,164,52,84,243,226,185,
3,168,75,152,228,242,1,66,250,224,7,200,158,77,180,84,254,130,24,6,24,230,78,41,0,201,164,197,245,47,181,241,157,231,219,72,218,6,255,61,181,142,163,135,199,72,103,131,45,127,0,13,117,150,30,50,40,38,47,21,195,106,98,189,165,217,122,62,141,65,32,241,
122,209,158,252,227,223,38,65,52,108,240,237,231,218,184,254,181,14,80,71,121,255,40,254,117,63,224,70,224,156,254,73,147,171,167,84,240,249,189,75,72,132,4,89,71,117,75,218,17,191,91,44,100,32,45,193,127,230,116,115,237,243,173,44,234,246,193,12,241,
229,47,158,207,231,206,255,60,19,198,79,32,22,143,23,163,61,30,4,126,241,168,64,130,89,194,227,143,222,207,25,103,157,75,58,157,201,162,142,191,95,218,217,4,0,160,6,229,161,247,9,75,144,28,83,105,51,185,46,204,144,18,11,211,52,88,214,229,50,179,177,192,
156,86,7,55,144,57,224,101,84,115,141,190,204,126,10,161,18,41,62,245,251,163,170,185,244,128,114,82,41,7,60,23,17,137,98,213,212,35,42,107,17,145,8,210,243,144,157,109,248,107,150,35,211,41,176,237,157,230,73,239,153,217,58,242,1,71,223,177,138,183,
91,28,38,213,132,121,238,220,122,146,33,67,165,28,139,237,247,179,13,161,2,84,166,33,54,88,193,249,129,202,182,220,30,5,95,219,66,34,98,242,86,67,158,233,247,52,208,156,245,30,71,165,237,78,0,254,5,140,2,248,193,33,149,252,223,49,213,184,221,46,121,111,
251,87,107,202,226,4,147,136,24,180,103,2,190,247,82,43,127,124,171,11,128,227,143,63,150,239,125,231,91,28,56,245,240,226,23,167,193,243,55,218,224,244,100,16,89,96,148,242,187,223,252,130,43,191,246,45,252,64,190,142,242,171,108,219,82,1,216,222,182,
224,205,168,164,140,19,61,201,5,239,181,58,163,223,107,117,170,81,110,60,160,10,34,154,81,89,125,183,3,255,222,14,159,225,8,224,147,251,212,132,57,119,92,18,55,231,33,157,60,86,117,127,236,125,14,198,168,170,67,34,16,66,32,165,42,251,51,59,91,113,223,
120,129,160,173,9,236,48,219,228,86,218,87,179,154,33,48,162,38,255,121,167,139,57,109,202,119,224,147,163,226,148,198,44,178,105,111,187,172,87,123,210,125,19,17,3,97,9,124,95,210,233,177,54,139,208,22,130,210,144,65,56,2,210,83,25,151,59,139,16,120,
94,192,184,90,155,169,253,195,252,111,145,119,12,240,19,84,181,103,93,207,215,116,228,60,130,180,167,210,197,183,33,238,176,225,127,244,204,154,27,222,18,181,226,18,196,19,22,243,27,10,92,242,68,19,207,174,204,81,85,85,193,149,87,94,201,215,175,186,130,
176,157,128,160,27,60,111,19,23,81,172,251,183,231,65,40,203,133,23,125,153,153,111,206,228,214,219,239,156,82,220,210,92,177,165,159,127,71,245,5,120,168,248,26,82,220,26,12,42,254,249,106,212,146,127,233,118,250,185,97,224,44,33,136,92,177,95,41,101,
9,147,76,87,22,171,170,31,225,169,199,34,74,202,145,133,28,4,114,221,54,64,8,140,202,58,66,147,15,165,240,194,99,80,200,128,101,127,236,34,16,139,26,44,106,200,243,155,215,59,113,3,24,90,26,226,243,123,149,224,58,1,158,236,251,6,163,82,66,52,36,176,108,
131,165,29,46,207,44,207,242,226,138,28,203,187,60,220,226,113,88,194,22,12,47,11,113,200,160,24,7,213,71,24,82,26,82,102,175,174,236,115,179,215,45,197,9,32,22,49,56,100,64,132,7,150,100,13,63,144,87,110,252,53,203,187,61,242,78,128,105,138,45,18,129,
158,71,193,48,84,77,134,41,54,92,37,75,36,126,160,142,38,131,226,196,109,26,16,139,153,60,179,32,197,151,159,108,101,126,135,195,62,147,246,226,198,27,127,201,17,211,142,1,153,7,183,141,94,249,229,9,1,94,150,104,172,146,31,93,247,83,222,120,115,22,239,
207,91,120,121,49,102,246,252,206,40,0,61,44,43,190,118,20,163,129,79,31,220,47,194,9,99,18,120,174,143,52,67,216,123,31,136,40,173,64,102,82,31,220,208,74,9,249,44,102,101,29,102,191,65,120,11,222,65,152,161,143,237,65,14,164,178,217,206,56,1,223,122,
182,141,133,197,2,168,107,14,44,103,96,89,136,238,180,191,29,42,252,212,224,241,125,201,239,94,239,228,143,111,117,50,175,125,115,39,38,57,110,122,167,155,209,229,54,103,143,77,112,197,228,50,74,163,6,217,124,240,177,38,87,249,190,132,0,142,24,22,167,
223,204,46,86,165,62,120,90,214,152,241,201,250,146,82,171,119,159,82,162,50,133,34,17,3,195,18,224,131,231,73,114,110,128,187,94,163,35,11,65,60,100,96,199,12,164,175,18,135,162,81,147,39,22,166,57,247,193,70,90,243,1,167,157,122,18,55,254,250,55,12,
30,60,20,188,14,181,199,23,91,114,35,13,240,186,24,56,104,40,215,92,243,45,190,112,225,151,201,23,156,235,129,131,217,130,36,33,139,221,155,211,129,216,41,163,227,84,197,45,50,109,105,204,65,35,48,235,6,128,147,255,72,165,53,98,113,37,243,114,211,235,
218,237,221,178,56,144,16,15,9,176,13,174,125,178,133,123,23,170,116,222,179,70,39,248,244,132,36,133,188,191,101,207,76,47,159,242,100,212,96,69,202,227,235,79,181,244,164,161,50,181,62,202,137,35,98,28,94,31,165,50,106,16,72,88,210,233,241,90,99,158,
39,150,101,153,177,38,207,181,47,183,243,212,178,44,127,57,190,150,209,85,246,90,11,181,143,77,60,125,201,144,18,139,254,49,115,147,2,208,94,8,112,148,71,237,135,63,7,66,5,75,237,176,65,0,172,105,119,152,221,230,241,118,83,158,247,90,29,22,116,184,180,
100,215,189,127,153,109,176,119,109,152,131,250,71,56,110,88,156,33,245,17,102,45,202,112,254,195,77,180,230,3,206,60,237,100,110,254,203,223,40,47,175,4,183,189,39,250,188,21,247,202,131,32,195,217,159,58,143,127,254,235,63,60,246,216,83,147,81,73,115,
183,107,1,80,249,220,231,15,45,177,56,126,100,2,92,181,63,13,13,24,6,102,8,89,248,8,1,232,73,137,221,104,14,235,9,197,36,108,3,12,240,61,73,206,147,125,90,246,43,165,218,243,39,98,130,84,65,242,189,199,91,248,205,204,78,0,166,244,139,240,243,163,170,
136,90,130,116,190,111,3,87,170,208,200,98,105,151,195,167,238,107,224,181,134,2,3,18,22,55,28,85,201,233,99,74,48,13,129,116,214,157,154,140,169,8,113,220,232,56,223,59,184,156,187,230,167,249,217,43,29,188,180,58,207,217,247,55,112,199,137,117,140,
173,10,147,42,124,76,137,86,197,214,230,21,97,131,189,106,195,188,222,244,65,227,29,115,51,219,148,158,123,25,50,139,109,210,129,198,92,192,43,139,83,60,184,40,195,11,171,213,86,168,120,152,213,138,242,149,116,81,197,166,98,5,216,179,91,157,178,219,231,
164,74,134,151,119,114,230,216,18,158,92,146,161,49,15,103,158,126,50,127,186,233,47,148,151,151,128,211,86,124,6,183,246,10,25,224,229,48,237,24,223,186,234,171,60,245,212,179,150,231,249,23,162,82,230,221,61,93,0,246,5,6,29,84,31,97,92,89,136,92,58,
143,40,173,192,168,170,3,55,223,187,39,72,6,31,216,251,11,161,210,96,95,111,204,51,175,213,225,132,17,113,42,19,22,153,140,255,145,238,65,189,89,98,10,169,202,149,205,136,201,220,53,57,174,126,190,141,7,84,87,93,198,86,132,248,235,241,53,12,46,179,73,
165,251,214,103,81,162,114,3,218,178,30,159,125,160,137,215,26,10,76,237,31,225,230,227,107,25,95,23,38,95,92,46,111,28,107,8,28,159,176,41,248,244,62,165,28,60,32,202,37,143,54,243,216,178,44,159,125,184,137,199,206,174,167,44,98,168,35,87,177,195,199,
63,142,47,9,199,77,198,84,217,189,94,173,73,9,33,75,16,137,152,16,4,188,219,88,224,129,69,89,254,61,55,197,252,118,167,103,208,207,6,22,161,186,81,63,85,220,214,182,174,247,35,170,128,3,37,156,181,168,195,61,250,167,175,180,245,3,56,241,184,163,249,199,
173,255,38,22,143,128,219,177,133,179,190,220,244,127,11,1,126,59,135,31,121,4,135,29,58,149,103,158,125,97,18,170,24,238,190,61,93,0,62,109,155,130,163,6,199,17,33,129,227,249,216,101,85,24,137,146,143,158,253,81,35,81,230,211,106,111,86,220,6,72,84,
231,155,214,156,207,37,79,180,48,171,169,192,113,195,98,252,97,122,45,67,171,108,50,41,79,157,35,111,129,16,172,239,109,24,13,9,172,152,73,58,23,112,219,140,118,126,245,70,39,139,84,71,35,14,234,31,225,230,233,53,76,168,182,201,164,189,62,191,88,161,
162,187,208,119,95,104,229,229,213,57,246,175,13,115,251,169,253,24,82,22,34,211,173,138,165,54,21,104,52,4,56,129,196,237,244,24,82,105,243,215,227,107,153,126,215,106,102,54,21,184,238,229,118,126,117,108,13,33,51,88,155,63,176,67,145,106,78,30,88,
98,17,181,12,178,31,242,33,122,116,62,25,49,32,100,240,234,170,28,183,191,211,205,195,75,50,44,239,246,64,249,247,61,138,58,111,127,162,248,255,155,163,117,189,192,247,65,168,140,189,227,187,211,25,186,187,219,137,197,251,245,98,214,95,207,209,85,8,48,
109,148,179,152,185,209,247,74,192,71,96,113,241,69,159,227,153,103,95,40,69,117,219,222,163,5,192,2,142,169,142,153,28,53,56,130,44,4,24,134,137,81,90,222,187,121,64,24,72,215,33,200,101,55,80,105,101,90,41,137,153,130,81,229,54,179,154,10,60,186,36,
203,201,119,173,230,155,7,150,243,153,177,73,68,216,192,247,36,133,30,83,8,241,225,179,84,216,18,132,12,101,224,209,158,246,184,231,253,20,119,205,73,109,208,96,228,11,123,149,112,237,161,149,12,40,177,72,103,253,190,247,44,144,16,137,154,252,119,110,
138,191,206,78,49,40,105,241,199,227,106,24,82,105,147,233,118,63,50,162,47,80,182,222,153,110,143,254,21,33,174,63,162,138,115,30,104,224,166,183,187,56,107,76,130,131,6,68,72,229,130,143,231,120,208,11,24,16,183,8,91,130,172,247,193,24,75,207,231,143,
218,6,33,83,240,94,171,195,159,102,118,240,191,133,25,26,148,189,249,34,148,105,236,115,108,93,47,195,87,81,199,143,63,123,225,197,87,47,254,218,149,87,112,243,45,255,32,153,8,131,231,108,250,102,24,22,152,209,226,96,87,31,50,151,110,101,233,210,121,
228,114,5,154,154,91,72,165,211,196,99,49,106,107,107,136,199,35,140,24,57,138,233,199,29,199,222,19,199,243,206,236,57,83,81,101,241,43,247,84,1,24,6,36,199,87,217,12,46,9,225,56,62,88,22,34,81,138,244,123,17,152,18,66,5,9,29,135,141,67,236,121,87,18,
183,5,127,60,182,26,219,132,219,230,164,120,175,213,225,162,71,155,185,107,110,154,139,38,150,112,200,128,8,229,113,11,97,9,229,23,86,76,226,18,197,109,27,197,1,47,3,73,42,227,243,78,135,114,211,125,96,97,134,119,91,11,107,125,20,71,149,219,124,239,224,
114,206,156,80,66,24,169,236,188,250,248,120,77,57,8,25,116,21,2,126,50,163,3,55,144,92,62,185,140,201,3,163,100,83,110,175,131,156,2,8,4,20,178,30,199,143,138,115,202,200,4,183,207,73,113,203,59,221,28,52,32,138,101,192,14,239,128,38,64,250,80,23,53,
41,11,27,116,228,55,188,247,53,81,147,152,45,48,98,38,29,157,46,127,154,213,205,159,222,233,98,141,90,97,181,161,114,7,238,96,219,253,249,186,129,111,2,131,255,115,231,255,142,59,242,200,35,185,240,139,95,1,209,190,110,62,18,6,88,97,32,10,20,104,105,
92,201,172,89,239,240,242,75,175,241,250,172,183,105,92,185,134,166,166,38,92,223,165,224,56,248,190,143,105,152,216,97,155,144,105,81,223,175,31,245,67,6,146,203,230,1,246,65,37,61,237,177,2,48,77,64,226,176,250,40,152,66,89,83,25,2,98,241,94,62,56,
2,233,185,72,207,101,109,255,175,245,180,33,227,72,74,35,130,91,166,215,176,119,77,132,95,191,217,193,202,148,199,67,139,51,60,180,56,195,164,154,48,199,12,137,49,182,210,102,64,194,160,95,60,68,121,68,69,206,155,179,62,45,57,143,213,233,128,197,29,14,
207,175,204,242,90,99,97,131,60,254,193,37,22,159,219,43,201,197,19,75,233,87,106,227,56,62,169,237,148,162,42,4,132,108,193,253,239,164,152,213,84,96,239,26,155,207,78,40,65,230,131,77,218,161,75,192,18,170,134,67,2,5,111,157,19,179,170,205,135,176,
16,156,59,174,132,127,205,73,241,244,242,44,11,218,28,70,173,231,88,180,35,241,165,36,30,18,196,54,113,212,87,159,52,137,68,45,30,122,63,205,53,47,180,50,187,197,1,101,195,117,11,240,59,84,146,90,95,145,2,110,148,112,248,239,255,112,83,236,212,83,79,
165,170,166,18,124,7,204,24,32,104,110,110,225,133,231,238,225,133,231,95,226,174,187,238,163,169,181,248,227,77,32,4,86,194,66,36,4,177,170,40,145,88,12,47,239,146,110,205,224,119,7,52,47,106,101,214,188,119,123,106,3,5,234,8,252,209,61,85,0,142,52,
13,97,239,93,27,6,3,130,32,64,152,33,140,112,20,130,45,28,29,155,249,227,76,94,98,26,240,181,131,202,56,126,120,140,223,190,217,193,19,203,114,44,238,116,121,187,185,192,219,205,42,234,108,25,80,27,55,41,9,153,4,18,218,11,62,237,121,255,3,133,59,117,
113,147,97,165,33,78,31,157,224,140,209,73,6,85,90,20,114,1,169,172,183,214,38,123,123,5,203,130,64,98,26,130,9,85,54,95,217,167,148,154,132,42,206,250,192,224,47,174,22,66,38,52,165,61,34,150,65,105,212,164,224,4,56,235,9,148,151,247,57,168,127,132,
113,149,54,11,59,29,222,108,41,48,170,54,12,249,29,111,188,208,227,3,185,169,197,71,214,149,92,246,72,35,183,205,73,81,240,101,30,101,13,127,45,240,254,118,250,56,79,1,119,189,51,123,206,249,207,63,251,52,167,159,253,57,48,163,204,125,255,61,238,190,
251,223,220,119,239,195,188,53,91,53,174,142,84,71,168,220,187,130,202,9,21,84,140,47,35,90,27,35,82,22,198,176,77,172,184,137,105,25,4,190,196,203,122,120,57,15,167,219,161,208,233,208,244,122,51,75,238,93,70,224,4,7,160,172,200,211,123,162,0,244,143,
90,130,250,132,9,82,205,84,70,40,68,95,30,154,11,161,246,144,233,140,207,152,106,155,63,30,87,203,220,230,2,143,44,202,48,163,177,192,219,77,5,86,165,60,242,190,100,117,202,103,245,70,85,229,241,144,160,54,110,49,174,194,230,128,254,97,142,28,20,101,
223,254,17,34,97,3,233,161,44,202,118,64,90,173,4,114,249,128,79,142,136,113,216,192,8,253,162,38,185,220,7,147,120,122,12,90,37,146,159,207,232,228,129,197,25,18,33,131,207,239,85,194,217,99,147,4,210,87,158,120,197,166,172,165,37,22,199,14,141,49,231,
77,135,89,107,242,124,122,108,242,227,217,6,124,8,15,45,201,246,152,124,204,64,213,215,223,187,3,126,236,125,192,105,79,61,243,124,201,222,251,236,207,157,119,223,201,95,254,252,119,150,173,80,171,245,234,253,170,168,157,82,67,205,254,213,148,140,72,
98,218,38,102,200,68,74,137,159,247,145,190,36,240,2,252,130,15,66,96,37,66,216,101,54,201,65,9,236,82,155,234,253,170,104,153,217,74,215,194,238,49,168,126,5,11,246,52,1,8,3,118,125,210,162,58,106,18,248,129,58,206,11,71,139,222,89,65,159,15,160,84,
198,39,100,9,198,86,217,140,173,9,83,200,7,44,203,120,180,102,125,26,186,61,150,118,185,56,197,241,31,15,11,134,38,66,212,148,90,148,71,13,6,68,76,18,81,19,164,74,31,205,102,252,181,75,239,29,21,52,11,164,50,12,25,152,180,200,187,18,79,202,15,196,153,
13,161,220,154,174,125,177,149,31,188,220,177,246,239,94,90,157,199,50,224,244,241,73,210,197,158,128,82,173,189,217,167,198,6,96,78,171,67,206,9,8,153,6,158,183,227,183,1,155,251,121,197,193,127,13,170,82,112,205,14,250,56,143,2,75,238,188,235,222,73,
207,61,247,2,243,22,44,6,1,3,143,29,192,160,233,3,168,24,95,129,93,98,33,165,36,112,37,110,218,197,113,29,85,212,35,84,23,140,13,238,140,27,224,21,171,54,157,180,139,16,130,104,109,140,174,133,221,195,81,253,10,246,56,1,168,1,74,6,196,45,42,34,38,190,
183,13,41,58,189,169,148,44,254,195,245,215,245,32,176,13,193,232,210,16,163,203,67,80,191,153,19,232,98,101,167,235,75,178,121,53,219,247,36,28,238,240,51,115,161,186,240,164,10,146,77,166,165,72,136,197,77,230,182,22,248,211,219,93,8,224,119,3,107,
105,241,124,126,208,208,202,247,95,110,227,144,65,81,170,108,131,172,167,30,84,233,6,140,40,83,2,176,38,237,209,236,74,6,197,12,149,158,178,131,127,191,205,220,197,23,129,171,217,241,29,133,28,160,161,163,179,107,82,71,103,23,253,142,168,99,244,217,35,
169,216,187,2,67,168,237,147,211,237,110,104,245,38,62,188,34,87,244,68,134,125,137,17,54,136,215,197,64,217,134,141,222,19,99,0,81,192,138,219,130,152,41,200,247,156,245,108,81,49,143,84,126,0,150,181,46,124,47,123,55,203,72,9,5,95,82,232,113,139,221,
68,40,97,109,146,97,239,66,14,31,251,76,41,0,97,9,94,111,40,208,150,147,28,24,143,114,105,191,10,218,61,159,199,187,211,204,104,205,243,194,242,28,103,142,75,34,92,117,82,81,240,97,120,121,136,178,176,73,206,13,40,4,114,199,23,6,20,51,42,91,114,42,238,
178,94,68,254,143,40,151,160,220,199,116,169,223,16,66,76,31,123,193,104,49,246,11,163,17,166,192,77,185,184,110,176,225,141,216,194,235,37,3,48,108,131,104,205,90,51,222,145,31,245,61,198,110,40,0,245,64,105,101,212,36,176,133,10,182,9,32,159,5,191,
151,206,117,82,34,162,49,140,72,20,124,111,91,159,65,2,185,225,75,238,66,23,83,2,33,11,112,3,222,107,44,224,7,146,195,146,49,8,2,42,12,193,180,100,28,1,188,214,144,71,122,235,50,5,37,146,144,1,21,17,65,151,35,233,46,54,72,221,225,34,23,18,172,201,248,
100,148,125,249,10,84,174,252,183,63,198,193,15,176,20,3,55,92,22,70,24,130,66,123,129,192,11,182,106,208,111,40,0,18,97,26,132,146,107,139,215,198,237,137,2,80,5,36,74,35,6,34,36,8,164,84,181,254,190,199,90,47,231,143,220,20,7,136,80,8,35,18,255,80,
215,221,61,5,211,48,8,124,88,216,165,178,18,199,69,67,107,29,114,247,137,133,9,11,131,185,237,14,110,32,49,214,41,0,150,33,232,151,8,145,118,3,178,57,127,91,159,239,173,195,18,44,233,118,201,170,26,230,235,80,217,121,31,55,13,72,220,192,241,215,13,252,
226,42,75,110,107,217,185,31,96,134,77,149,131,242,17,134,161,187,171,0,120,128,239,175,191,206,22,42,207,85,230,179,189,159,130,2,137,40,171,0,59,164,210,129,247,84,228,186,139,218,211,229,169,220,52,214,94,218,132,80,251,200,188,191,110,151,181,97,
214,186,196,20,235,58,57,203,29,248,177,67,166,0,87,50,183,197,193,87,237,19,95,222,73,174,106,51,42,127,23,2,48,163,38,225,138,48,161,68,136,72,121,88,237,247,183,242,66,5,18,194,37,54,86,164,119,150,118,187,99,12,32,7,184,65,241,240,87,244,232,156,
239,35,179,105,122,117,109,165,90,171,27,85,253,16,86,8,233,56,136,112,100,167,112,6,250,56,227,3,61,147,251,250,45,255,220,158,193,182,25,93,245,2,21,109,239,89,49,136,29,37,2,18,236,144,160,51,231,51,179,177,208,51,232,150,239,36,151,179,14,176,164,
148,88,113,147,212,178,28,243,255,189,136,108,67,150,218,41,213,212,31,214,31,191,224,35,183,178,183,162,44,246,136,220,83,5,96,37,208,217,148,241,203,41,72,44,83,40,39,98,223,71,118,119,20,205,62,123,81,41,41,125,68,178,20,163,172,146,160,113,213,30,
61,248,55,179,40,80,219,3,161,14,166,252,77,40,134,31,64,71,62,160,42,106,80,85,22,218,161,43,0,1,24,150,96,126,147,199,50,181,117,121,17,85,182,187,115,196,169,36,33,43,102,225,23,2,102,94,255,54,205,111,180,0,176,234,201,213,4,5,201,144,19,6,82,232,
116,182,106,207,36,132,232,245,66,119,119,220,2,228,0,39,235,4,248,126,160,126,65,33,144,129,143,204,116,247,254,33,247,149,75,171,49,96,152,74,32,146,193,30,63,232,123,146,120,44,33,214,102,215,213,218,33,194,134,96,117,218,195,11,214,229,15,4,82,29,
135,254,240,144,114,190,63,181,146,33,113,147,92,33,216,97,2,96,22,139,230,158,90,156,166,81,229,39,252,155,190,78,2,217,134,56,149,97,11,51,222,63,206,138,135,87,208,252,70,11,251,238,61,145,47,93,248,57,252,188,207,242,71,86,224,164,93,140,136,177,
21,131,31,220,140,135,223,203,158,156,187,163,0,180,0,217,166,172,79,103,193,87,15,2,2,33,12,252,84,39,65,46,141,48,123,177,63,146,18,129,196,26,48,28,81,94,181,205,167,1,187,242,218,95,74,149,255,95,22,86,143,75,198,87,149,125,82,8,108,33,48,128,148,
179,174,59,238,186,93,148,228,156,241,73,46,152,84,138,236,201,147,216,65,31,59,108,26,116,164,124,30,89,146,5,229,61,57,115,39,186,170,147,140,144,105,32,36,203,159,86,125,110,175,251,233,181,220,244,231,223,210,175,178,142,212,234,52,185,230,28,86,
216,220,226,37,147,48,13,220,172,75,160,142,20,67,59,131,0,20,75,25,216,81,198,122,105,32,189,50,229,209,154,151,24,61,150,94,86,136,160,187,131,160,179,173,215,78,191,210,115,17,201,82,172,97,99,213,126,108,15,221,6,72,41,49,132,96,64,82,237,24,151,
57,158,26,200,114,221,148,186,57,119,157,84,54,32,181,61,74,152,63,66,180,12,19,158,94,150,101,166,114,2,250,19,219,94,209,215,151,12,8,197,44,242,109,14,173,111,181,49,121,210,68,246,159,114,48,18,155,218,154,42,124,127,43,247,255,2,164,175,82,134,139,
44,250,56,4,160,10,213,168,224,207,168,162,138,249,192,188,226,107,33,240,44,112,17,202,29,120,123,60,18,18,200,117,22,2,154,243,30,162,71,118,12,3,10,14,65,91,11,189,126,26,101,128,240,92,66,195,199,97,214,212,43,35,145,93,174,141,232,118,216,253,111,
193,37,248,56,50,27,67,66,208,237,5,252,101,118,55,5,95,118,161,12,60,118,150,229,127,12,72,70,235,162,164,87,165,9,10,1,135,29,62,149,202,138,26,178,221,93,228,11,5,12,195,216,170,94,28,194,16,72,63,192,233,116,122,45,0,125,21,4,140,3,7,160,76,56,47,
4,204,120,72,152,73,219,160,196,54,9,91,170,246,61,227,73,186,11,193,136,182,188,127,56,80,0,238,2,126,3,204,234,227,139,60,211,11,228,49,139,219,92,113,216,160,117,15,160,8,133,8,214,44,69,142,154,128,176,108,164,91,248,136,167,83,32,93,7,17,137,18,
218,231,96,130,231,31,66,58,5,132,21,218,35,135,127,79,12,192,220,132,44,172,127,74,240,113,19,142,153,60,248,110,138,199,151,101,65,213,243,191,177,19,93,198,137,64,77,114,80,146,206,101,221,24,66,176,223,190,147,64,8,60,223,193,247,131,205,167,138,
246,66,0,2,87,146,111,203,131,138,116,47,217,222,2,96,0,211,129,11,128,211,5,176,87,149,205,193,3,162,28,53,56,202,196,218,48,101,182,129,64,32,132,196,149,176,172,211,227,145,197,105,241,232,210,108,100,102,99,225,60,148,91,202,207,81,53,216,125,117,
76,243,168,47,249,242,155,13,249,210,207,239,85,130,101,244,120,2,152,4,157,29,248,109,205,88,245,131,193,235,197,19,43,4,178,144,199,172,170,37,52,102,111,156,183,94,5,45,0,197,129,47,73,24,42,14,144,114,3,58,243,1,117,9,243,99,155,107,131,98,247,164,
246,180,199,143,94,109,7,213,120,230,175,59,217,101,156,10,212,197,235,163,180,190,216,74,109,117,53,163,198,78,0,160,185,169,153,214,150,54,66,213,54,102,212,68,110,97,233,164,176,4,158,227,145,105,204,129,42,110,90,188,61,5,96,31,224,235,192,185,150,
33,56,118,112,148,79,141,75,50,109,80,140,250,82,107,237,35,18,4,235,207,167,130,126,9,139,131,6,69,184,114,138,207,127,222,79,241,187,55,187,66,243,58,156,107,80,173,141,126,2,220,211,7,23,121,54,144,125,181,177,80,154,247,2,66,134,192,13,64,24,6,184,
14,254,178,133,88,181,245,8,195,68,246,50,201,71,122,46,230,128,225,152,75,230,17,100,82,96,134,216,19,82,4,69,113,96,97,64,93,204,4,1,237,126,80,252,115,73,76,168,198,24,89,71,210,154,11,168,79,90,31,203,117,145,18,108,3,76,203,224,199,207,180,50,187,
165,16,0,191,69,25,119,238,76,140,49,35,38,86,212,34,179,34,203,168,17,195,168,239,95,15,192,146,37,203,232,204,118,81,89,81,129,157,180,9,188,45,168,159,144,96,88,6,110,218,165,208,158,7,88,213,155,9,117,107,99,0,95,2,254,7,156,59,181,62,194,189,167,
212,241,159,51,250,243,153,73,37,244,79,152,228,242,62,233,156,79,42,27,144,201,175,123,165,243,62,169,156,79,54,235,83,106,27,124,121,74,25,15,156,213,159,47,79,42,65,8,246,65,249,153,255,30,213,179,125,91,232,2,50,203,187,92,102,53,59,152,150,177,129,
245,146,191,102,9,65,75,35,132,236,45,152,254,124,140,68,9,70,221,64,229,20,180,7,133,2,130,98,153,98,117,212,68,0,141,142,135,193,58,179,141,158,189,190,241,49,157,41,245,108,65,34,49,213,58,173,216,111,239,53,84,141,255,206,68,21,48,182,100,88,18,51,
98,33,211,146,254,117,117,84,87,171,78,121,115,231,47,66,34,137,247,143,17,74,170,46,75,91,162,212,61,117,5,78,151,67,49,246,214,186,61,4,224,151,192,111,163,150,24,124,221,161,21,60,112,102,127,78,26,151,36,26,64,58,237,147,46,30,7,173,31,0,218,224,
85,156,81,178,133,128,116,218,103,100,121,136,95,79,175,229,142,147,234,24,85,110,135,129,75,81,37,154,39,108,229,231,139,160,252,215,250,119,228,3,94,89,147,87,33,234,181,107,30,19,153,203,224,46,122,79,245,2,180,182,96,17,100,90,16,141,179,193,178,
102,15,220,2,236,116,157,129,37,36,74,44,102,54,22,248,206,139,109,61,238,62,151,0,153,157,236,147,238,11,236,83,54,178,116,109,111,133,17,35,134,97,217,113,64,242,246,172,183,1,40,29,94,138,48,197,22,185,167,8,161,210,85,186,150,164,122,130,128,115,
129,124,95,10,128,137,178,75,250,218,208,82,203,190,243,148,126,124,251,144,74,202,44,131,116,198,35,95,44,106,16,91,240,129,1,82,57,159,192,9,56,123,92,130,71,206,232,199,103,198,151,16,15,25,163,129,251,139,65,194,67,81,205,68,63,236,173,195,168,190,
131,103,163,206,123,127,150,12,137,152,4,158,91,158,165,80,8,136,132,68,241,146,27,96,71,240,87,45,198,95,177,8,66,225,45,95,19,239,161,172,237,137,183,209,236,219,35,135,31,87,16,48,25,183,88,221,233,114,249,99,205,44,237,246,188,226,224,127,103,39,
188,132,227,129,88,229,94,21,120,57,149,87,82,94,86,2,8,90,27,87,240,236,179,47,96,196,13,42,38,148,19,184,91,88,127,98,8,132,41,232,94,220,77,160,132,99,101,111,190,109,75,98,0,167,0,223,27,148,180,184,227,212,126,28,48,32,74,186,15,252,233,5,202,20,
195,205,73,134,150,135,184,237,228,90,254,55,55,206,111,222,236,48,95,107,40,156,94,240,229,233,40,107,229,23,138,251,154,86,148,169,2,168,211,135,1,168,118,207,39,1,149,117,113,147,147,71,196,57,126,100,130,139,30,105,226,245,134,2,111,181,21,56,176,
95,132,130,235,131,144,42,138,239,228,113,223,153,129,81,90,129,40,175,132,92,254,195,7,183,44,142,128,130,67,223,247,227,218,69,226,1,197,58,10,99,189,75,98,10,136,10,65,71,209,20,69,238,72,17,144,144,136,153,116,228,124,46,125,180,89,173,246,224,6,
224,159,59,225,229,139,0,7,89,49,147,234,253,170,88,249,236,106,0,236,72,2,128,7,31,121,140,53,13,205,148,140,46,161,116,88,114,203,150,255,168,229,191,151,15,200,172,201,128,202,133,89,214,151,2,80,5,252,56,110,9,227,231,71,85,113,192,144,24,153,46,
183,207,175,80,58,31,16,178,224,180,241,9,142,29,26,227,193,69,105,30,89,148,225,149,134,252,65,171,83,254,65,133,77,44,137,76,1,213,81,147,145,21,54,7,215,135,57,121,100,130,131,7,68,33,102,114,199,123,221,220,57,47,205,83,11,51,28,88,31,197,52,85,126,
186,138,24,69,144,169,46,10,111,62,79,120,234,39,48,34,113,100,33,187,233,86,77,82,34,108,155,32,211,141,191,102,153,18,144,61,40,39,40,8,212,168,175,76,152,216,134,160,217,13,138,6,52,146,18,211,96,64,40,196,138,130,199,234,140,207,36,99,189,77,249,
118,220,243,19,64,50,110,210,89,240,185,232,225,38,238,95,156,1,184,13,248,17,155,40,77,216,9,24,11,28,87,49,161,130,196,192,248,218,100,157,120,162,4,223,43,112,203,159,255,130,39,61,134,158,52,8,187,196,38,223,94,216,162,107,104,88,6,217,134,44,221,
203,82,20,183,208,243,251,82,0,142,4,198,158,61,38,193,25,99,147,56,105,143,237,97,240,98,20,91,159,103,210,62,49,219,224,83,19,74,56,115,84,130,69,105,143,198,110,159,85,157,46,171,210,30,94,177,70,63,30,22,140,46,179,169,47,15,209,47,106,80,19,87,81,
106,199,9,176,253,128,115,199,38,184,115,94,154,135,22,103,184,104,223,82,170,35,38,89,39,88,47,108,28,38,104,92,137,243,234,211,132,15,58,10,17,79,170,146,97,185,225,26,69,216,42,88,232,189,247,6,178,171,29,236,8,123,146,2,244,180,72,140,135,13,76,1,
109,158,191,182,172,79,20,29,215,37,170,67,208,14,89,137,0,137,184,201,202,180,199,165,143,54,243,160,26,252,255,6,46,99,231,41,248,217,152,201,64,162,102,223,106,236,132,189,214,55,97,238,251,115,248,206,213,223,226,181,215,102,82,54,178,148,129,71,
215,227,229,124,181,175,218,130,133,166,17,50,72,45,79,145,90,158,6,149,116,215,217,151,2,112,86,121,216,228,156,9,73,117,228,227,202,237,22,8,234,89,102,102,10,1,134,163,106,186,71,151,132,24,83,102,195,160,232,38,6,158,178,231,245,3,73,174,160,2,144,
106,2,18,76,29,16,229,160,254,17,94,107,200,243,236,210,28,231,236,85,130,112,131,117,25,189,66,32,34,49,252,134,229,20,158,127,132,208,222,7,98,212,244,223,176,215,187,148,248,93,109,120,239,205,196,91,181,164,152,4,180,103,166,4,175,245,63,148,18,47,
0,67,8,60,169,250,2,32,119,76,94,185,101,64,36,106,49,187,33,199,5,143,52,247,164,250,254,21,184,124,39,30,252,9,224,43,209,154,40,53,251,87,227,102,61,170,38,84,130,1,127,251,219,109,20,60,7,43,110,178,215,101,227,177,75,195,228,219,243,136,45,9,168,
8,144,158,164,115,65,39,210,147,244,118,246,223,18,1,152,62,176,196,100,66,185,141,116,119,140,183,91,143,16,56,190,196,233,241,215,219,68,190,249,166,146,166,4,224,6,146,138,132,197,153,163,226,188,186,38,207,237,239,119,115,242,232,56,150,16,184,27,
228,244,11,68,56,66,208,222,68,225,197,199,176,250,15,66,84,214,168,100,114,223,195,111,111,65,182,172,33,200,101,16,33,123,143,221,255,227,73,250,69,77,34,150,96,133,227,209,224,122,12,140,132,232,246,3,26,92,15,9,148,111,69,241,74,175,87,32,64,204,
54,48,195,38,143,204,237,230,178,167,91,89,210,233,186,192,205,192,149,244,194,253,230,99,100,34,48,177,116,68,9,101,99,202,112,83,46,165,99,202,24,245,233,17,44,125,104,57,165,117,37,140,191,96,44,213,251,86,81,232,42,108,217,224,47,62,195,190,235,211,
242,86,27,168,228,159,231,250,90,0,172,129,73,139,154,132,133,227,125,124,179,223,150,100,72,6,82,53,188,56,117,76,146,219,231,102,120,120,73,150,231,151,229,56,110,100,12,39,187,241,10,70,168,101,125,16,224,173,90,12,43,215,37,80,73,1,66,152,8,59,194,
158,140,239,73,134,21,173,214,151,228,92,102,229,10,12,44,75,48,167,171,131,6,215,163,42,97,48,166,34,132,231,245,237,254,95,181,74,135,88,196,164,203,9,184,233,133,86,126,246,90,7,29,133,160,19,101,233,253,199,93,224,242,93,33,12,65,253,97,253,176,34,
6,185,86,7,97,8,198,125,97,12,131,167,15,36,92,30,38,92,22,198,77,187,91,37,160,102,72,144,94,153,161,115,94,23,168,228,159,94,55,54,233,237,116,38,66,166,80,182,250,219,208,165,70,202,205,188,182,211,190,181,224,72,134,86,219,28,53,40,10,192,117,175,
117,144,246,138,13,46,54,245,67,13,3,44,91,37,8,21,95,194,178,139,197,229,123,46,66,64,174,16,80,89,18,226,232,193,49,60,224,223,237,41,230,116,103,249,77,115,7,221,65,192,153,35,227,212,36,76,28,191,111,124,255,123,186,38,199,34,6,177,132,197,27,107,
242,124,250,127,141,92,253,66,27,29,133,96,62,112,206,46,50,248,251,1,211,19,245,113,106,167,214,174,109,238,33,125,137,95,8,72,12,78,96,70,76,10,93,133,173,242,3,148,129,196,46,181,105,122,173,5,55,237,6,192,235,91,180,165,234,229,215,57,43,187,61,187,
45,227,83,17,49,85,27,168,45,188,153,162,103,9,103,27,197,202,17,122,58,27,227,20,124,10,125,92,43,46,128,176,45,88,211,229,241,210,42,101,0,251,242,170,28,255,157,155,226,115,251,150,226,117,121,104,122,79,0,120,110,192,165,251,148,112,247,188,52,247,
180,119,243,102,58,199,18,199,101,120,89,136,203,246,41,239,185,157,219,124,31,123,26,145,196,147,22,153,140,199,47,95,233,224,207,111,119,177,90,29,59,255,13,184,30,85,89,186,43,112,5,144,168,59,184,150,68,191,24,185,150,220,122,75,124,137,87,60,74,
23,91,147,68,33,193,8,153,184,25,143,166,215,155,65,213,62,252,119,123,8,192,171,75,58,221,99,222,106,117,56,122,88,76,157,194,139,222,13,66,203,20,216,150,192,23,176,164,205,225,221,54,7,199,151,116,20,36,81,83,165,151,78,169,11,83,145,52,49,60,200,
123,65,159,180,143,178,76,129,48,224,119,175,117,240,106,67,30,84,113,68,245,141,175,119,132,166,15,141,81,19,51,73,231,131,157,47,171,109,39,197,16,144,119,2,198,85,217,252,245,19,53,124,251,165,86,230,180,186,76,238,23,230,55,71,86,51,178,42,68,58,
183,237,179,191,105,8,162,150,192,55,224,193,121,41,110,124,173,131,231,86,230,65,69,181,47,71,37,135,21,118,145,203,150,4,78,136,84,69,140,225,103,13,67,24,2,43,106,169,148,146,64,130,47,9,182,193,39,94,6,146,72,153,77,199,220,14,58,23,116,130,74,254,
153,181,61,4,224,206,142,66,112,204,191,222,235,230,232,161,49,34,182,65,222,217,252,224,145,18,44,75,16,181,13,186,243,62,15,206,203,112,219,251,41,94,111,200,211,146,245,123,246,232,82,160,50,30,135,151,219,76,27,20,229,252,9,73,246,175,143,96,32,200,
228,183,174,55,158,148,16,178,4,145,184,201,45,175,117,242,179,215,58,64,29,139,156,10,92,56,187,197,185,234,134,215,58,249,197,49,213,132,204,0,47,208,131,123,75,200,20,36,39,141,73,112,64,255,48,75,186,61,70,151,219,148,199,12,210,249,173,187,144,61,
125,87,236,144,192,182,77,252,64,242,210,202,28,191,121,179,147,123,23,166,9,36,169,226,160,255,30,59,151,169,71,111,184,12,152,144,239,202,243,250,79,223,36,86,19,163,124,120,25,165,195,75,136,247,143,17,173,142,18,142,91,4,5,31,63,31,224,187,193,22,
197,152,69,72,128,1,141,47,55,227,166,60,80,217,179,91,116,35,68,111,246,29,66,136,1,192,203,17,83,12,250,211,244,26,206,159,82,142,159,246,200,110,198,119,44,25,49,193,22,60,179,40,195,141,175,117,240,240,210,44,40,175,190,183,80,71,20,13,69,69,79,162,
18,36,70,2,123,39,109,193,197,123,151,114,241,190,101,12,171,182,113,115,1,121,55,248,200,184,131,92,251,15,72,70,12,8,25,220,57,187,139,43,158,110,163,41,235,21,128,207,160,170,12,235,128,217,53,49,179,250,238,83,251,113,216,208,24,169,148,167,87,1,
91,177,244,140,218,6,166,37,112,93,73,193,221,242,174,191,61,143,93,216,18,216,17,3,47,128,119,26,242,252,243,221,20,127,121,183,171,167,145,199,11,168,218,147,7,118,193,171,148,44,14,200,105,229,229,229,184,133,2,233,236,186,83,74,187,46,68,253,148,
254,84,78,44,167,118,74,13,177,254,113,2,39,192,233,118,84,131,143,143,218,18,72,176,226,22,129,27,240,210,149,51,104,155,221,22,20,199,209,146,13,175,179,236,19,1,0,56,6,184,167,44,108,148,124,255,144,10,190,56,169,148,88,216,4,55,192,47,54,220,48,13,
1,33,88,211,237,241,187,55,58,249,243,219,221,61,45,153,238,6,254,3,60,194,166,11,20,250,161,60,5,206,2,38,142,175,180,249,230,65,229,156,51,58,137,29,18,184,197,246,211,155,202,51,17,66,53,160,176,45,129,97,27,52,167,60,110,124,189,131,223,188,209,73,
94,237,37,46,0,254,190,222,183,92,0,252,245,224,254,17,238,59,171,63,21,182,65,246,99,232,91,191,203,107,192,250,125,235,182,36,150,80,244,23,12,135,4,102,200,192,115,3,158,93,157,231,214,183,187,120,124,89,142,86,213,64,228,57,84,195,206,59,233,69,65,
203,78,202,103,129,191,157,121,250,201,214,69,95,250,10,174,147,103,217,162,185,188,251,254,92,22,46,92,204,172,153,239,210,209,221,9,64,217,232,82,250,31,82,199,128,99,6,80,50,52,73,80,8,112,51,238,186,125,244,102,8,151,135,105,120,161,145,215,190,255,
38,94,214,123,2,229,205,33,183,151,0,0,156,1,220,4,84,30,61,56,202,153,99,146,28,90,31,101,64,194,4,33,89,222,237,243,196,242,44,183,191,151,226,173,230,66,207,158,228,123,40,87,22,167,23,23,109,40,240,101,224,202,144,33,204,99,135,196,248,218,148,50,
166,213,71,16,97,149,162,43,253,226,83,84,220,227,3,224,73,154,51,62,247,45,206,240,247,217,93,204,88,83,0,104,71,21,133,220,181,241,54,19,149,43,254,169,175,238,87,198,47,143,173,198,43,4,107,115,13,52,219,79,48,44,67,16,137,26,8,36,169,76,192,127,23,
165,185,107,94,154,25,171,243,116,168,213,228,12,148,49,204,131,40,115,215,93,149,90,224,129,68,34,62,229,217,103,30,103,242,254,83,55,8,167,182,183,181,177,104,225,123,188,242,202,171,60,240,192,163,60,251,252,75,0,196,234,162,12,56,122,0,67,79,30,76,
201,208,4,78,183,75,224,108,122,117,101,88,2,195,54,121,235,103,239,176,236,225,21,0,199,2,79,126,240,186,247,173,0,0,236,143,106,172,120,116,200,16,86,69,212,32,106,9,132,132,140,39,105,205,249,4,18,183,56,235,127,159,94,248,146,109,252,187,161,42,0,
127,9,236,83,22,54,140,201,117,17,78,27,21,103,108,149,77,101,196,164,196,22,116,20,2,186,11,1,43,82,30,47,175,202,241,212,178,28,43,186,61,156,64,186,168,246,79,215,160,74,34,55,69,41,240,223,144,33,142,250,221,49,213,92,184,79,9,185,92,95,196,175,53,
27,7,129,13,67,16,50,192,10,155,248,94,192,187,45,5,30,90,148,225,174,121,25,22,180,59,20,124,233,1,207,23,7,254,83,64,219,110,240,171,95,8,220,114,222,103,206,226,175,127,187,13,203,114,193,43,128,17,82,9,102,194,66,21,176,66,103,71,11,175,190,242,2,
183,252,249,86,30,120,224,33,124,36,177,250,40,99,206,27,205,208,147,6,67,0,78,218,217,176,190,66,66,40,105,145,90,145,225,165,43,94,33,215,146,159,7,28,182,41,209,220,30,2,208,19,60,220,7,101,231,117,220,122,193,68,175,184,95,187,173,56,248,182,37,196,
22,65,85,248,93,7,148,10,168,49,4,148,69,12,42,35,22,13,25,143,172,27,80,172,237,73,21,103,252,57,192,119,128,119,63,226,103,215,160,76,75,79,49,5,220,117,74,63,62,57,38,78,58,171,79,5,182,101,134,23,168,148,9,211,16,216,69,155,224,156,15,45,25,159,167,
151,102,120,104,113,150,71,150,102,200,171,100,178,85,168,162,149,95,161,60,251,118,151,254,107,253,128,247,235,170,171,202,30,122,248,62,246,219,255,32,112,59,55,138,88,21,11,206,132,40,250,81,68,8,2,201,19,79,60,202,13,63,251,37,207,62,247,34,1,146,
1,71,213,51,238,162,49,148,12,74,224,102,60,213,71,176,104,252,17,46,179,153,243,151,121,188,255,231,121,160,156,185,110,220,244,125,217,62,2,176,163,153,84,20,155,81,168,246,223,81,148,217,67,182,24,80,124,18,213,249,165,163,23,239,117,6,202,122,108,
228,136,178,16,159,25,87,194,57,99,19,12,45,179,40,108,199,26,135,221,52,22,168,90,112,11,8,135,85,80,16,41,240,131,128,197,109,46,51,86,231,121,122,69,150,231,87,230,88,222,237,129,42,83,125,167,184,199,191,169,40,2,187,27,191,1,46,191,234,202,175,240,
139,27,127,7,50,5,222,71,156,155,75,9,150,9,70,25,249,124,138,155,254,120,19,191,188,241,70,86,173,110,36,49,48,193,94,151,141,99,224,209,3,112,58,29,188,156,143,157,12,225,102,29,158,187,248,37,82,203,210,93,192,52,84,128,125,183,21,0,54,177,143,223,
210,25,195,40,174,38,174,182,12,184,120,239,18,46,221,191,156,49,85,97,164,19,144,118,244,236,191,37,193,63,33,32,108,10,66,81,19,164,164,59,31,176,176,195,229,185,165,89,102,54,22,120,187,165,192,220,182,181,97,159,23,139,179,253,203,40,139,238,194,
110,122,121,246,3,222,28,53,114,40,79,63,253,12,3,6,14,4,183,131,94,37,220,74,84,105,101,40,1,132,121,243,141,87,185,230,154,111,243,196,147,207,99,197,45,198,93,56,134,209,231,14,199,73,123,132,226,33,22,223,187,148,183,126,246,14,168,196,168,139,217,
76,191,187,143,26,223,187,106,111,64,127,43,4,227,15,192,151,70,149,133,184,110,90,21,103,140,77,128,47,201,100,61,130,0,61,248,123,193,218,211,22,0,67,208,145,245,120,113,69,150,215,86,21,120,97,101,150,229,93,30,43,215,153,196,204,66,101,165,205,41,
6,247,154,246,128,75,244,51,211,52,184,252,178,175,48,96,224,16,240,59,122,31,87,90,91,106,153,2,145,99,242,254,7,113,199,29,119,241,163,31,92,203,175,127,247,39,102,255,230,61,220,148,203,196,203,198,147,109,202,177,232,206,37,61,219,168,91,232,85,179,
203,221,75,0,182,116,230,255,29,240,165,41,117,17,110,57,174,134,137,3,34,56,89,159,66,177,178,81,15,254,205,92,56,161,154,127,134,67,2,108,3,39,23,176,186,219,229,173,102,135,255,45,204,240,102,99,158,101,93,30,105,55,144,168,83,158,183,128,251,128,
167,81,69,41,45,123,208,229,58,15,152,118,208,129,7,240,197,47,125,25,188,110,213,86,126,139,171,71,13,144,62,184,109,84,84,86,242,139,27,127,77,125,125,61,223,254,206,255,49,247,111,243,177,75,109,140,144,32,165,140,63,102,22,197,117,171,217,85,183,
0,91,50,248,191,7,92,59,165,95,132,127,157,84,199,136,10,139,116,62,88,27,180,210,251,120,245,15,33,214,13,248,144,41,48,12,129,52,160,203,147,44,107,46,240,74,67,158,215,214,228,121,108,105,134,214,92,64,32,201,160,34,246,239,20,103,250,151,138,131,
126,79,44,178,136,1,15,89,150,57,237,190,123,255,205,9,39,157,5,110,43,219,238,144,32,33,20,1,226,220,118,235,45,92,122,233,149,164,179,25,204,176,137,159,247,83,192,105,168,147,147,15,217,178,237,158,91,128,222,114,32,112,109,191,152,201,45,211,107,
24,81,21,34,173,58,197,234,6,95,69,1,12,153,16,178,12,149,196,37,0,63,96,89,218,99,126,155,195,115,203,114,204,106,46,48,171,185,64,171,74,225,206,1,111,163,234,42,254,131,10,230,181,162,57,31,152,118,246,153,159,100,250,113,39,130,236,166,111,236,81,
4,184,121,48,125,206,251,220,69,68,163,97,190,248,197,203,232,236,234,6,21,248,126,106,155,183,117,187,243,150,21,248,70,200,16,124,231,144,74,38,14,138,144,221,195,43,0,123,38,131,144,41,136,132,133,178,215,241,37,89,71,50,175,41,207,27,13,121,94,111,
204,51,179,209,97,78,91,161,167,78,162,9,85,98,58,27,213,100,227,9,118,94,231,157,143,131,58,224,139,241,120,140,75,47,189,24,211,138,129,219,214,135,83,140,80,167,8,118,192,17,135,31,77,191,254,181,61,2,240,189,190,26,36,187,43,195,129,83,166,212,133,
57,127,124,2,63,23,224,125,136,109,149,89,108,131,189,59,214,6,169,165,189,74,191,37,100,34,11,1,11,219,92,222,107,115,153,177,50,203,171,13,5,150,117,185,172,74,121,61,121,164,11,138,123,249,121,197,25,127,1,59,159,199,254,206,194,17,192,222,103,124,
242,68,38,31,48,21,100,166,143,215,151,18,172,48,96,112,243,205,127,100,241,226,101,160,58,30,189,191,43,11,128,0,198,21,247,48,83,233,73,139,130,21,197,217,230,73,84,6,225,182,140,199,201,128,152,62,44,70,34,106,146,78,251,155,246,173,151,16,9,9,60,
95,109,132,13,54,93,115,176,43,32,232,217,203,11,34,166,80,103,31,82,217,117,119,23,2,94,92,157,231,229,134,60,51,27,10,188,221,84,160,35,239,147,86,254,104,110,113,176,255,13,213,81,103,5,42,191,66,231,71,127,56,113,224,252,120,60,202,185,231,157,79,
200,138,128,219,222,247,119,213,72,242,214,172,25,252,238,247,55,225,56,110,39,42,75,150,93,85,0,198,3,87,3,103,11,33,66,85,149,229,68,34,17,60,223,39,147,201,208,221,157,62,15,85,0,242,48,42,75,236,149,173,124,16,247,78,216,6,19,170,194,16,108,218,169,
90,74,72,198,76,22,183,57,124,233,137,22,14,238,31,225,154,131,202,49,165,196,221,73,151,2,27,95,136,144,161,188,15,44,179,120,156,97,66,222,131,149,157,14,11,58,92,222,111,115,120,113,85,158,89,77,121,86,167,124,242,190,244,81,149,152,237,168,40,242,
195,168,168,125,43,219,112,156,180,135,50,12,248,196,212,131,166,112,192,148,3,139,33,146,190,244,68,87,65,64,215,201,114,195,207,127,78,115,75,27,197,177,179,98,87,21,128,115,128,191,0,241,131,15,152,204,169,167,159,198,161,135,28,76,101,69,57,78,193,
97,197,170,53,188,245,246,91,60,253,212,83,145,23,95,124,245,116,215,243,79,47,238,117,126,89,188,186,91,66,194,18,144,8,9,54,229,255,37,139,51,127,214,11,184,242,217,86,158,94,158,165,127,209,86,124,173,231,245,86,238,177,183,197,54,109,227,64,29,172,
107,171,102,8,176,138,81,122,12,148,89,158,23,208,230,4,180,166,125,22,183,20,120,187,201,97,97,167,203,59,45,121,222,106,90,155,136,211,4,44,45,206,234,207,23,103,249,183,216,125,19,114,118,216,242,223,48,12,142,61,230,40,74,74,203,193,235,128,62,247,
181,138,115,239,127,255,206,127,238,190,159,226,100,248,191,190,14,148,237,40,190,4,252,182,36,25,183,191,121,213,21,124,233,226,75,169,170,233,95,252,43,15,48,24,63,113,50,199,29,127,50,87,92,126,25,247,220,115,23,63,251,217,47,153,59,111,209,143,138,
91,132,239,111,225,150,96,113,218,13,88,216,233,114,180,105,108,240,173,170,50,13,66,182,201,15,159,109,225,193,69,25,134,150,134,184,230,224,114,108,75,40,103,27,177,101,3,223,18,16,141,25,197,230,65,82,117,201,97,157,158,244,214,207,160,231,182,135,
77,129,105,9,204,98,78,253,218,191,247,37,75,59,93,150,167,84,210,205,59,77,5,222,106,114,88,209,229,178,38,227,145,85,121,246,126,113,118,127,179,56,248,159,45,14,248,180,30,179,125,58,58,207,239,87,87,195,145,71,29,173,110,144,12,250,80,0,2,8,149,179,
124,233,60,190,241,205,239,18,4,65,7,170,53,95,243,174,40,0,23,2,191,28,60,168,222,254,253,111,111,224,196,83,206,81,131,222,235,80,73,15,61,23,77,74,48,76,226,137,56,159,59,255,98,246,219,111,10,103,159,245,41,222,159,183,224,27,168,130,145,45,49,134,
120,195,11,224,133,149,57,190,184,79,41,182,41,214,149,252,10,136,38,44,238,125,55,197,47,222,232,36,108,10,110,60,170,138,209,245,17,50,157,91,110,16,98,25,130,130,47,153,223,84,160,54,102,210,47,97,17,177,45,101,123,228,4,248,190,196,9,62,24,91,232,
105,173,21,46,158,187,11,83,40,37,1,40,4,116,57,1,109,133,128,53,41,143,121,45,14,115,218,28,222,111,115,88,217,237,209,146,243,123,106,231,41,14,236,55,139,1,187,183,139,179,253,98,96,181,30,167,219,141,4,48,182,190,190,142,241,227,199,247,237,238,73,
6,96,69,64,250,124,231,59,215,176,114,213,26,80,213,181,79,246,245,47,177,35,4,224,52,224,183,3,250,215,69,255,254,183,191,48,237,168,233,16,164,193,239,89,125,174,23,151,23,197,229,186,155,6,145,103,194,94,251,242,227,31,255,31,167,157,241,153,48,202,
44,228,9,122,111,16,49,27,88,245,236,138,220,128,215,27,11,28,52,48,130,155,86,3,38,17,55,153,211,84,224,219,47,180,82,240,37,223,57,168,156,83,199,38,201,117,251,172,175,225,18,213,115,222,178,13,100,160,60,241,54,222,225,73,9,145,152,193,226,118,151,
243,31,106,34,231,73,142,24,24,101,76,181,205,176,82,155,33,73,139,250,132,73,105,212,80,203,142,245,68,64,10,240,221,128,150,92,64,123,222,103,77,218,103,85,198,101,85,218,103,73,155,203,130,118,135,165,221,46,121,79,146,113,37,5,181,127,247,81,69,79,
207,21,151,243,75,81,46,48,45,64,55,187,79,85,221,206,78,25,224,15,25,52,8,59,18,5,217,71,2,32,37,24,22,24,9,126,125,227,245,220,113,231,189,20,69,253,27,219,227,151,216,222,2,48,22,184,53,18,9,71,127,253,219,95,169,193,239,119,65,240,81,230,241,2,164,
7,100,57,228,208,163,57,240,128,253,152,241,218,204,253,129,254,108,100,121,244,33,116,2,215,55,101,253,223,255,252,149,118,254,117,90,63,34,97,3,164,36,147,15,248,214,83,45,44,232,112,57,113,120,156,255,59,184,130,66,214,199,11,214,57,19,7,18,18,97,
131,206,66,192,21,15,54,225,7,240,199,99,171,136,91,6,249,245,28,140,141,98,91,230,229,173,5,22,118,184,100,61,201,130,14,119,237,223,85,70,76,170,99,38,165,97,67,237,221,229,134,122,151,247,36,157,69,111,131,182,188,223,179,117,240,129,84,241,85,64,
101,216,245,20,212,188,87,252,243,44,187,231,169,229,174,66,63,192,28,54,108,40,96,130,204,247,205,224,55,45,48,75,121,240,254,255,242,195,31,95,143,148,116,160,92,172,186,119,53,1,72,160,90,54,37,190,241,245,203,57,253,244,115,32,72,65,224,210,187,44,
41,1,50,160,180,52,201,184,177,163,153,241,218,204,26,148,145,199,150,112,7,112,238,125,139,50,7,253,226,229,14,190,63,173,18,4,252,248,233,86,30,94,154,101,112,137,197,13,71,85,17,178,4,153,245,246,253,18,84,61,187,37,248,217,243,237,220,254,126,138,
49,21,33,213,130,185,103,149,82,252,90,195,84,181,221,47,173,200,247,236,191,215,237,226,36,169,150,156,255,70,75,206,183,138,159,125,227,190,98,162,56,192,51,168,92,250,158,96,93,19,234,156,119,118,95,239,249,52,125,70,6,8,58,58,58,215,169,249,182,134,
125,13,1,102,41,175,207,120,145,175,95,245,13,58,58,186,2,224,91,108,166,212,119,103,23,128,175,2,83,62,113,236,145,124,251,219,223,133,32,91,92,246,27,91,116,81,66,33,40,73,38,123,198,229,150,134,230,219,139,193,199,71,126,58,163,125,192,136,50,147,
146,168,197,207,95,239,32,105,27,252,225,216,26,70,215,132,201,108,100,12,106,8,136,196,76,254,251,94,55,191,159,213,69,137,109,112,227,180,42,202,108,147,116,193,223,224,107,195,38,52,116,56,60,190,124,147,201,113,127,1,190,166,166,8,170,81,38,39,27,
11,64,174,248,57,245,17,220,174,197,106,64,188,251,222,28,2,191,128,97,218,108,253,161,74,160,102,126,163,140,215,95,125,158,207,156,119,62,11,23,45,3,248,1,170,218,143,93,77,0,14,2,190,81,87,91,99,254,246,87,55,16,141,151,128,219,82,28,7,91,18,103,21,
56,78,64,87,119,215,182,124,150,119,129,243,10,190,188,227,146,39,91,107,163,38,120,18,174,159,90,193,9,163,226,228,50,254,6,35,82,2,241,136,201,156,198,60,151,63,221,66,214,147,252,232,144,10,142,27,157,32,151,253,96,12,192,48,13,30,89,148,226,157,150,
15,220,252,69,192,79,139,255,237,3,141,122,204,236,86,116,1,139,22,46,94,50,113,198,140,215,57,120,234,225,219,48,248,99,96,196,120,228,225,7,185,228,146,139,89,177,114,13,168,108,191,31,111,239,95,194,216,78,239,249,5,160,228,91,223,188,156,81,227,246,
41,158,143,110,77,123,45,65,54,147,103,213,170,134,158,61,253,214,166,163,62,11,156,155,114,130,150,230,92,192,144,164,197,241,99,146,96,25,170,49,3,27,46,253,29,55,224,154,231,219,88,147,246,57,126,72,140,43,167,148,226,59,18,79,110,216,189,40,98,9,
58,178,62,127,122,167,107,227,8,191,143,74,98,106,209,227,100,183,69,2,191,104,105,105,231,79,127,252,3,129,239,64,40,182,101,139,84,1,132,74,145,216,252,249,166,63,240,153,207,158,199,138,149,107,2,148,99,213,55,119,68,140,103,123,8,192,97,192,5,83,
246,159,196,185,159,189,72,45,139,228,214,102,148,134,104,107,111,103,254,130,69,160,90,65,109,75,229,217,211,192,209,192,146,197,93,46,231,220,187,134,231,230,167,137,68,77,18,182,88,151,47,31,49,248,201,171,29,220,191,40,195,136,178,16,215,31,83,77,
60,98,146,115,215,89,135,75,169,202,102,173,168,201,141,111,116,246,180,168,94,63,213,248,45,148,229,149,102,247,230,97,224,189,219,255,125,55,63,250,193,15,240,61,11,66,165,69,15,128,96,35,49,144,235,94,134,165,156,127,172,10,22,45,90,204,249,159,251,
44,151,124,249,43,116,116,116,54,3,23,161,12,109,119,72,146,86,95,11,64,8,184,40,28,182,197,229,151,93,74,117,117,13,184,91,91,56,38,1,155,57,179,95,103,197,202,213,160,142,66,58,182,241,243,205,6,38,7,146,127,204,108,42,200,147,238,93,195,213,79,183,
178,50,237,19,139,155,196,42,109,30,91,156,225,23,175,119,96,10,248,225,33,21,236,85,23,33,157,9,214,106,152,148,106,149,16,45,179,184,251,189,110,126,50,163,157,170,168,201,25,163,19,36,195,107,47,231,21,232,8,253,158,64,7,170,20,184,229,218,31,253,
132,111,95,125,21,173,45,109,96,149,65,168,4,204,176,114,1,54,76,48,109,53,232,67,149,96,150,208,216,216,202,159,111,250,53,199,30,251,9,110,187,253,63,4,146,89,40,19,220,191,237,216,117,140,148,31,249,218,2,38,1,242,168,105,83,101,119,87,151,148,50,
47,165,211,42,165,211,182,229,47,191,83,250,94,65,94,116,225,121,61,61,39,63,213,199,191,250,249,197,153,90,238,91,19,150,255,56,177,86,190,121,209,96,185,95,109,88,2,242,171,147,75,165,188,122,164,204,126,117,184,236,190,124,152,76,93,49,76,166,46,31,
38,131,111,141,148,242,187,163,229,61,167,246,147,253,226,166,4,228,13,211,170,228,79,14,173,144,182,41,36,170,95,189,102,207,226,136,226,10,85,78,59,226,16,121,231,127,110,147,77,141,171,229,198,116,119,181,201,153,111,190,46,127,252,195,239,202,169,
7,79,233,89,18,164,81,54,251,85,31,199,248,238,107,71,160,127,132,109,251,115,127,248,221,13,124,225,139,151,129,223,9,193,214,76,132,18,66,101,44,93,244,62,147,15,56,156,246,246,142,215,129,19,183,195,158,250,23,192,85,160,150,255,253,227,22,43,211,
30,147,170,109,94,254,244,0,98,73,11,10,254,186,35,30,67,176,164,195,229,183,51,187,248,219,59,93,164,220,128,239,31,92,193,81,131,98,124,242,254,6,218,114,254,251,192,39,216,61,221,110,53,31,206,104,212,201,215,197,145,136,205,132,113,99,217,119,191,
73,84,85,84,32,132,32,157,78,49,251,189,247,89,188,120,89,207,138,22,224,231,168,236,214,151,183,231,4,191,163,86,0,181,64,122,252,184,145,178,113,205,50,41,165,43,165,211,178,117,179,191,215,46,165,148,242,87,191,252,73,143,74,254,110,59,92,155,47,1,
29,195,75,45,249,135,163,171,229,129,253,35,107,55,105,165,97,83,30,59,36,38,175,220,175,76,222,112,68,165,188,113,90,149,252,201,97,21,242,172,209,73,57,40,25,146,168,36,62,249,189,67,42,101,235,85,35,228,180,129,209,158,239,189,88,143,131,61,154,80,
113,53,112,39,235,142,118,189,226,203,45,206,246,51,138,66,49,129,29,96,76,245,145,99,187,15,5,224,74,64,94,249,213,75,213,122,199,235,218,202,229,127,171,148,65,70,102,83,141,114,175,9,99,36,170,130,109,98,31,95,151,35,129,85,81,75,200,187,78,174,147,
242,187,163,100,247,21,195,228,63,79,172,145,167,143,78,200,218,152,37,109,67,200,13,35,55,234,149,12,25,242,248,97,49,249,244,167,234,165,252,193,24,249,189,169,21,61,127,119,71,241,1,208,104,12,84,243,153,253,128,19,138,171,194,3,129,193,59,250,25,
249,168,177,109,245,225,47,124,90,73,50,193,73,39,156,160,198,67,224,108,101,140,81,130,136,241,159,187,254,201,220,121,11,41,42,230,236,62,188,38,251,2,183,134,12,81,255,243,105,85,156,185,87,9,185,180,71,72,192,103,38,150,241,233,241,37,188,215,226,
48,191,195,229,221,134,60,157,133,128,144,41,112,3,201,240,114,155,253,250,133,57,160,46,76,168,44,196,163,111,119,115,131,106,63,222,90,220,78,232,100,30,13,168,0,112,51,187,64,22,103,95,9,192,80,96,248,136,145,35,56,120,234,193,16,100,182,126,240,155,
81,10,249,12,127,255,251,63,240,60,223,99,51,45,143,182,146,225,168,40,235,128,111,77,41,227,210,253,203,112,242,1,94,177,47,64,119,202,195,182,4,19,235,194,76,236,31,225,204,49,9,85,20,208,83,143,107,169,90,2,36,52,172,201,243,245,103,91,200,121,210,
47,6,113,102,233,231,94,179,171,209,87,2,48,29,168,58,252,208,169,132,163,165,219,102,137,108,196,121,234,201,255,50,115,230,91,160,172,166,95,234,163,207,56,1,184,21,216,251,170,253,203,248,238,225,149,248,78,176,65,59,48,195,0,55,144,184,105,31,65,
177,91,147,16,42,90,42,192,205,123,88,134,192,180,5,215,188,216,206,220,54,23,224,241,237,20,163,208,104,118,25,1,152,6,132,142,61,102,90,49,222,33,54,63,195,175,237,196,209,99,97,101,160,178,4,13,84,170,188,203,237,183,255,139,108,174,0,202,9,168,47,
124,233,198,163,90,130,79,250,250,254,229,92,119,84,21,102,209,13,119,227,3,142,181,134,64,80,180,5,91,247,227,77,3,194,113,147,63,191,209,201,173,239,117,131,42,218,249,138,126,140,52,123,178,0,68,128,234,146,146,36,251,76,218,139,117,125,33,54,170,
121,9,217,197,31,103,173,247,99,125,10,5,7,215,113,112,220,20,158,39,120,107,230,76,94,120,241,21,128,249,192,11,125,240,249,198,1,127,19,48,233,107,251,151,113,195,145,149,120,94,64,198,145,155,54,9,221,88,12,214,251,109,98,177,16,239,52,228,249,201,
171,237,61,45,208,191,132,170,222,211,104,246,88,1,24,1,212,31,124,224,254,148,87,150,3,78,209,192,46,4,134,77,143,225,175,12,114,52,183,116,208,222,222,193,202,165,243,88,178,120,9,43,27,26,104,88,221,72,123,107,59,45,173,29,116,118,118,210,149,238,
98,77,67,19,168,51,210,109,173,129,158,134,234,9,56,246,170,253,203,184,126,90,21,158,39,201,185,31,61,248,55,88,183,72,136,218,130,246,140,203,85,79,181,244,116,186,253,35,240,168,126,132,52,123,186,0,76,6,6,141,31,63,14,59,92,166,150,243,150,218,255,
183,182,172,100,246,187,243,89,48,127,46,51,223,120,141,121,11,22,179,122,197,26,86,174,92,131,183,81,7,41,81,42,176,19,54,133,53,5,80,71,127,175,108,227,231,58,23,248,53,80,245,205,3,202,249,233,81,85,4,142,36,235,109,249,224,15,153,2,43,100,240,157,
39,26,121,74,149,253,206,66,117,26,118,244,35,164,217,211,5,96,48,16,58,116,234,129,32,66,172,90,185,132,151,95,126,153,39,159,120,146,183,222,126,135,101,203,86,208,222,99,154,0,136,114,65,217,222,165,148,14,46,33,49,48,78,188,95,130,80,220,34,90,29,
161,115,81,138,55,127,56,19,25,200,251,80,169,149,91,203,87,129,235,45,131,240,143,14,173,228,170,131,42,8,92,73,206,9,182,120,240,11,3,34,73,139,91,94,235,224,207,239,166,64,89,50,95,128,174,244,211,104,1,0,138,46,61,47,191,50,131,103,159,127,153,123,
238,190,143,246,246,118,114,142,42,102,138,12,12,51,240,192,1,148,14,45,161,98,92,25,201,33,73,204,136,133,97,10,132,85,116,216,17,96,69,76,86,191,208,128,84,117,181,51,217,58,111,59,27,85,134,251,165,178,176,48,127,126,100,53,23,78,42,197,119,3,114,
155,8,248,125,20,134,128,120,194,226,169,69,105,190,243,66,27,82,146,3,190,139,106,136,169,209,236,241,2,32,80,230,136,252,226,198,223,171,63,9,67,233,176,18,134,30,54,148,202,189,42,40,25,148,32,86,31,199,46,177,241,29,159,192,241,113,58,93,2,199,7,
167,120,196,102,40,175,188,206,249,93,160,140,51,222,219,138,207,50,10,117,106,112,226,168,114,155,63,124,162,138,163,135,39,200,103,60,220,96,203,29,155,164,132,68,194,100,126,147,195,101,143,53,247,56,240,222,136,58,77,208,104,180,0,160,90,35,149,11,
83,144,28,150,164,255,212,58,106,246,171,166,124,108,25,86,204,68,24,6,129,23,32,3,201,210,251,150,209,57,175,139,88,255,40,3,142,238,15,66,32,125,185,206,243,94,72,10,237,133,30,1,104,218,138,253,254,79,128,65,199,13,137,241,243,35,171,153,80,107,147,
73,123,4,114,43,6,63,168,118,98,249,128,175,62,213,204,188,118,23,148,159,192,15,244,35,163,209,2,176,225,247,71,19,245,113,14,186,110,127,202,199,148,225,164,92,252,130,143,147,114,33,0,187,204,102,213,83,107,120,227,71,111,226,231,84,101,160,25,50,
24,56,125,160,26,240,18,12,219,192,47,4,120,121,15,182,44,133,114,4,202,115,239,146,176,41,248,202,190,101,252,223,161,21,148,216,6,233,108,177,13,248,86,204,252,225,176,32,39,225,138,39,90,120,108,105,22,84,64,242,12,116,170,175,102,55,99,91,13,65,18,
64,153,21,53,85,4,191,195,193,237,118,9,10,65,49,171,215,164,107,65,55,179,174,127,155,184,17,231,208,169,7,146,136,198,89,124,255,82,156,180,139,81,52,208,8,149,134,104,153,217,74,122,85,6,160,178,103,91,241,17,194,243,13,224,33,224,146,125,107,194,
252,231,228,58,110,56,186,138,184,41,72,109,228,243,183,37,131,63,100,8,108,211,224,167,47,182,241,55,149,236,51,15,21,244,235,212,143,139,70,11,192,134,148,1,245,129,47,241,114,222,186,168,128,64,121,29,134,77,22,223,183,4,167,219,225,180,211,78,228,
159,183,221,76,191,126,181,116,44,235,196,75,185,8,67,96,151,217,180,191,219,193,91,63,127,7,95,237,179,247,6,206,219,196,103,51,81,166,9,95,44,14,202,159,149,134,197,232,171,166,148,241,240,25,253,57,117,76,18,167,16,144,41,4,8,99,203,235,44,165,84,
153,126,145,184,201,175,94,235,224,103,235,138,124,174,66,37,37,105,52,122,11,176,17,61,195,253,3,105,115,102,204,36,189,58,195,138,167,87,82,94,82,194,249,159,255,2,131,135,141,193,52,5,126,119,128,151,243,40,25,146,160,123,89,154,89,63,123,155,92,75,
158,125,38,77,160,165,165,205,90,181,186,225,39,168,242,201,135,81,167,1,165,192,161,192,177,64,172,34,98,136,233,195,226,124,99,74,25,123,215,133,241,60,200,228,60,213,84,101,43,42,172,37,96,89,130,104,204,228,159,111,117,241,127,47,181,227,6,210,5,
190,93,252,12,26,141,22,128,77,16,176,137,227,58,25,72,236,18,155,21,143,174,196,239,14,152,124,244,190,28,122,216,84,90,26,27,113,28,15,44,176,226,33,140,136,201,251,127,157,71,199,220,78,198,142,26,197,227,143,61,194,188,121,243,249,246,119,174,17,
243,230,45,56,165,173,189,243,20,211,52,241,125,245,35,198,84,216,28,63,52,206,233,163,227,28,60,40,138,20,144,205,250,91,21,232,91,127,240,155,66,16,141,155,220,251,110,55,151,63,213,66,90,21,1,124,23,229,235,175,209,104,1,216,12,173,192,50,96,208,6,
251,138,144,65,224,6,180,205,109,71,32,56,236,176,195,48,204,8,43,86,174,32,155,205,18,169,10,19,171,141,178,226,137,213,172,124,114,53,201,68,130,155,110,254,53,213,181,3,169,174,29,192,19,143,79,228,213,25,111,177,166,161,153,87,94,124,134,155,110,
249,7,163,202,67,220,115,70,127,198,215,216,224,73,10,197,74,62,195,216,182,166,44,6,16,139,155,60,191,36,195,165,79,182,208,89,8,64,69,251,127,174,31,15,141,22,128,15,167,0,100,215,219,8,32,3,137,21,183,40,180,23,104,123,175,157,88,56,194,212,131,14,
0,96,209,162,133,116,117,119,83,113,104,5,133,182,28,115,110,122,31,63,239,243,173,31,92,206,97,71,28,7,126,7,4,146,88,34,193,81,71,127,2,240,185,231,158,59,1,248,218,228,50,198,215,216,20,178,190,234,242,43,85,249,238,182,6,64,98,113,139,23,150,102,
249,204,3,77,52,102,124,80,13,25,174,213,143,134,102,79,96,91,131,128,46,80,112,83,46,78,151,171,218,91,3,134,101,224,164,93,10,237,5,18,241,4,35,70,14,3,224,205,55,102,146,43,20,168,24,93,206,234,23,27,233,120,191,147,67,167,78,225,75,151,92,166,222,
170,167,187,134,155,5,36,207,60,241,32,15,60,240,48,227,43,109,142,31,158,64,22,2,10,158,106,206,177,45,179,190,44,198,44,98,113,147,103,151,101,57,235,190,53,172,74,123,0,127,6,190,174,31,11,141,22,128,222,145,1,90,221,148,139,155,114,214,53,204,180,
4,126,222,39,240,37,101,229,73,6,14,30,70,87,87,27,51,94,125,3,4,68,202,35,44,125,112,5,166,105,114,201,151,191,68,85,117,29,120,235,53,253,49,44,192,225,214,127,221,13,192,169,163,226,12,44,183,212,224,223,70,27,197,158,61,127,34,97,241,204,210,28,159,
123,176,145,166,108,0,170,145,199,197,176,81,149,146,70,163,5,224,67,233,112,51,30,78,151,83,236,148,187,193,60,139,48,76,192,230,149,87,94,225,237,217,239,145,24,157,32,179,58,77,199,123,29,28,119,236,52,78,56,225,147,32,11,32,253,117,223,103,70,105,
92,181,148,251,30,122,140,114,27,78,25,149,84,11,3,95,110,147,141,106,79,83,143,88,194,228,145,249,105,62,255,80,35,43,83,94,207,224,191,132,190,49,31,209,104,246,40,1,72,3,20,186,92,176,214,31,158,170,141,182,29,10,227,228,187,248,215,109,183,145,206,
102,25,48,185,31,107,94,111,192,14,133,56,251,156,115,40,41,45,7,47,189,46,136,128,4,194,60,240,240,83,116,119,116,178,87,117,152,137,213,33,124,111,219,198,102,32,33,108,10,194,49,147,127,189,221,205,185,15,52,178,66,13,254,63,21,7,191,70,163,5,96,43,
88,2,184,233,85,105,252,156,143,48,4,190,19,96,151,132,8,39,195,172,89,185,134,79,127,250,92,238,249,239,253,148,143,46,35,82,27,165,107,78,138,137,123,141,231,228,83,78,0,242,27,78,188,134,9,72,158,125,238,5,144,1,211,71,36,8,71,77,242,78,176,213,203,
255,64,66,216,18,216,17,131,223,206,232,224,146,39,154,123,162,253,223,3,46,213,143,129,70,11,192,214,51,11,88,153,89,153,193,203,122,24,150,129,155,118,137,213,69,25,48,173,63,173,169,118,254,251,191,135,241,99,48,238,139,99,104,155,171,218,251,157,
118,218,169,148,148,214,129,159,93,239,99,72,48,227,116,181,55,241,222,220,249,8,96,191,218,48,4,224,111,229,2,64,74,72,68,4,150,41,248,209,11,237,92,245,76,43,41,39,200,3,151,161,218,47,235,101,191,102,143,165,47,252,0,22,2,43,58,22,116,13,115,210,30,
209,138,48,210,145,120,89,159,145,159,26,1,8,58,23,119,51,230,51,35,73,14,75,240,230,245,179,40,77,36,57,237,180,147,213,216,91,107,187,221,131,205,194,69,75,104,106,104,164,206,134,9,21,54,126,33,216,242,212,222,226,187,38,19,38,221,121,159,171,159,
105,225,79,111,175,53,242,188,12,184,91,223,126,141,22,128,109,199,1,218,114,205,57,210,203,83,196,170,195,8,161,78,1,172,168,197,132,47,143,5,33,176,19,33,22,252,103,49,133,54,135,195,166,77,101,212,168,81,32,115,155,124,195,85,171,86,147,78,167,152,
92,101,209,175,52,68,38,191,101,203,127,41,149,165,119,52,102,178,168,213,229,171,79,52,243,176,170,234,155,95,220,239,63,171,111,189,70,211,119,237,193,95,2,220,182,183,218,48,195,166,106,39,38,32,240,2,220,180,135,87,124,181,190,213,6,1,156,112,252,
116,132,25,7,63,191,225,236,111,40,61,106,108,90,77,62,151,103,120,121,24,97,10,130,94,182,39,147,61,251,253,144,202,235,127,100,65,134,83,239,89,211,51,248,159,4,78,209,131,95,163,233,123,1,184,15,104,110,158,213,138,151,246,49,44,99,221,206,90,130,
17,54,200,182,228,232,90,220,133,133,193,228,253,246,94,55,85,175,63,124,133,9,192,202,21,43,145,82,50,164,52,4,94,239,150,255,178,248,143,120,212,192,178,12,126,253,106,7,231,62,208,200,156,54,7,224,22,224,108,116,85,159,70,179,93,4,96,25,176,188,107,
73,55,109,115,218,9,87,132,123,188,253,212,62,35,98,146,107,205,147,110,200,48,114,196,112,198,77,216,11,200,109,246,205,10,174,242,221,72,88,189,255,120,150,33,72,198,76,86,167,124,206,123,176,145,111,60,215,70,103,33,232,2,190,140,106,222,209,161,111,
183,70,179,125,4,0,224,175,126,206,103,213,211,107,16,66,96,134,205,117,145,56,67,144,105,202,226,167,125,198,140,31,77,34,89,241,193,229,63,128,97,32,165,75,163,234,11,64,93,194,90,247,30,155,217,235,11,148,125,151,29,49,184,111,94,138,163,254,189,138,
127,189,159,194,11,228,124,224,40,212,57,191,182,239,214,104,182,179,0,252,15,232,110,120,177,129,246,247,59,176,203,108,101,248,105,10,8,36,157,11,187,0,24,53,114,56,150,101,109,230,240,205,192,119,93,58,58,212,100,93,30,49,55,187,220,151,18,236,144,
65,60,105,177,180,219,229,235,143,183,240,233,135,154,89,212,233,22,80,101,188,7,161,220,133,53,26,205,14,16,128,14,224,87,249,246,2,243,111,91,72,224,4,88,81,11,33,4,210,135,220,26,181,228,31,57,114,56,226,67,202,248,132,16,24,197,191,15,54,51,235,91,
2,146,49,19,203,132,127,189,221,197,169,119,173,225,55,179,58,201,121,193,187,40,131,208,139,244,146,95,163,233,197,214,185,143,223,239,70,224,132,213,207,174,153,188,112,92,25,163,63,59,18,63,231,227,187,62,249,174,60,2,131,177,99,71,1,222,70,1,192,
141,6,121,32,55,57,235,155,168,163,61,33,224,249,101,89,110,124,189,147,199,150,101,112,84,25,193,245,168,106,62,221,171,79,163,249,152,4,160,27,248,126,224,5,15,205,185,121,174,8,151,133,25,113,246,48,156,46,7,55,227,98,34,168,168,172,217,204,220,14,
200,0,211,178,169,169,173,1,160,37,163,10,243,12,33,136,217,2,76,193,251,77,5,254,50,187,155,91,223,237,166,163,16,72,224,101,224,251,168,227,61,157,213,167,209,124,140,2,0,240,8,112,158,159,247,111,158,121,221,91,49,55,237,50,224,152,254,56,25,135,250,
126,253,136,39,147,168,214,224,124,240,24,80,6,96,88,84,87,87,2,144,113,3,136,154,32,125,94,111,200,243,239,57,41,254,249,126,154,142,188,15,234,72,239,6,224,86,180,93,183,70,179,211,8,0,192,237,128,23,120,193,207,222,254,229,236,65,203,30,92,78,118,
77,142,154,241,85,196,227,37,64,24,172,240,70,43,129,158,69,62,148,149,149,0,176,50,229,113,255,236,110,110,127,191,155,199,150,102,73,187,18,84,91,174,7,81,205,57,243,250,22,106,52,91,143,144,189,200,178,19,91,239,194,177,47,202,191,255,28,128,178,210,
18,206,58,235,116,142,57,250,8,6,12,28,70,121,121,57,65,80,244,1,16,6,233,84,55,77,141,77,220,114,203,205,60,240,208,227,36,66,162,103,208,3,60,94,124,221,129,234,30,164,209,104,62,130,143,26,223,219,91,0,0,66,168,243,248,227,81,209,249,72,216,182,169,
172,44,39,158,136,173,11,248,9,65,62,151,163,179,179,139,116,102,109,146,208,235,192,253,168,138,195,25,232,230,28,26,205,46,39,0,235,111,55,134,162,218,137,31,80,124,85,179,206,86,92,0,107,80,103,247,239,163,178,11,215,160,156,135,53,26,205,199,37,0,
26,141,102,247,196,208,151,64,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,
52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,
52,26,45,0,26,141,70,11,128,70,163,209,2,160,209,104,180,0,104,52,26,45,0,26,141,70,11,128,70,163,5,64,163,209,104,1,208,104,52,90,0,52,26,141,22,0,141,70,163,5,64,163,209,104,1,208,104,52,187,39,255,63,0,251,9,211,62,187,88,73,220,0,0,0,0,73,69,78,68,
174,66,96,130,0,0 };

const char* icon_png = (const char*) temp_binary_data_12;

//================== in.png ==================
static const unsigned char temp_binary_data_13[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0\x18\0\0\0\x18\x08\x06\0\0\0\xe0w=\xf8\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0\x0b\xaciTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:"
"meta/\" x:xmptk=\"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl."
"org/dc/elements/1.1/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmlns:stRef=\"http://ns.adobe.com/xap/1.0/sType/ResourceRef#\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0"
"/\" xmlns:xmpRights=\"http://ns.adobe.com/xap/1.0/rights/\" xmlns:tiff=\"http://ns.adobe.com/tiff/1.0/\" xmlns:exif=\"http://ns.adobe.com/exif/1.0/\" xmp:CreatorTool=\"Adobe Photoshop CC 2017 (Windows)\" xmp:CreateDate=\"2013-10-04T17:30:37+03:00\" x"
"mp:MetadataDate=\"2019-01-27T13:44:43+01:00\" xmp:ModifyDate=\"2019-01-27T13:44:43+01:00\" dc:format=\"image/png\" xmpMM:InstanceID=\"xmp.iid:ab4eebb0-4282-5447-843b-c1bb55cee103\" xmpMM:DocumentID=\"adobe:docid:photoshop:59c2099f-80fd-0340-b0b0-8a23"
"11633278\" xmpMM:OriginalDocumentID=\"xmp.did:05bd1fa1-dd19-8d49-a673-1ddd603b57d4\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpRights:Marked=\"False\" tiff:Orientation=\"1\" tiff:XResolution=\"720000/10000\" tiff:YResolu"
"tion=\"720000/10000\" tiff:ResolutionUnit=\"2\" exif:ColorSpace=\"1\" exif:PixelXDimension=\"24\" exif:PixelYDimension=\"24\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"created\" stEvt:instanceID=\"xmp.iid:05bd1fa1-dd19-8d49-a673-1ddd603b57d4\""
" stEvt:when=\"2013-10-04T17:30:37+03:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:97feccd1-2f2d-3245-a56f-9f16d415b29e\" stEvt:when=\"2013-10-04T17:30:44+03:00\" stEvt:softwareA"
"gent=\"Adobe Photoshop CC (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:43d83052-e5e0-3c42-8178-157680ade203\" stEvt:when=\"2017-01-04T13:30:33+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2017 (Windo"
"ws)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"converted\" stEvt:parameters=\"from application/vnd.adobe.photoshop to image/png\"/> <rdf:li stEvt:action=\"derived\" stEvt:parameters=\"converted from application/vnd.adobe.photoshop to image/png\"/"
"> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:bd1e9f9b-1220-eb47-acb3-f57632bb1b8d\" stEvt:when=\"2017-01-04T13:30:33+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2017 (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\""
" stEvt:instanceID=\"xmp.iid:ab4eebb0-4282-5447-843b-c1bb55cee103\" stEvt:when=\"2019-01-27T13:44:43+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> <xmpMM:DerivedFrom stRef:instanceI"
"D=\"xmp.iid:43d83052-e5e0-3c42-8178-157680ade203\" stRef:documentID=\"xmp.did:963eb1fc-5696-f44c-bde6-5f8b30db2291\" stRef:originalDocumentID=\"xmp.did:05bd1fa1-dd19-8d49-a673-1ddd603b57d4\"/> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end="
"\"r\"?>R\x08\x9d\xff\0\0\x02\xa4IDATH\x89\xb5\xd6O\x88\x95U\x18\xc7\xf1\xcf\xfb\xde?\xce\x84\xe3\xe4L\xa3\x0e\xfd\x99\xc4\xfe\xa0\xe0\x84\x16\x04\x16m\xa2\x10*\t\xda\x04!Y\x8a\xe4>\t\n"
"B\x14\xdc""E\x14\xfe\xa9U\xd4\xa2\xa0""6Z\xd4\xb2""emt\xa3\x0b\x83 b\xa6""B.9N\xe9""0z\xe7\xde{Z\x9c\xf7\x1d\xdfy\xef{kZ\xf8\x83\x03\xef=\x7f\x9e\xefy\x9e\xf3<\xe7\xdc$\x84\xe0v\xaa\x9e\x7fl\xfd\xa1o\xac\x81\xc7p\x08\xbb""1\x8e""4\x1b\xeb\xe1O|\x8b\x93"
"\xb8\x80\xa5\xe2\xe2K\xbbJ\x80\x92\xd6\xe3ML\xe3QL\x94\xc6Sl\xc0\xb3\x18\xcb\0\xef\xe1\xaf\x81\x1e\x14t\x1f^\xc3\xdb\x03\xe0""E\xdd\x9d\xb5\xe7\xd1\xc6\xa7\x98-\xef\xa4\xa8\x06\x0e\xe2\xc8*\x8c\x17U\xc3""1\xbc\x9e\xd9\x18\x08x\x03\xfb\xfe\xa7\xf1\xa2"
"\xf6""a\xff \xc0$\xf6\x88./\xeb\xef\x0e\x0b\xddU\x03\xee\xcfl\xdcU\x05x\t\x0f\x96Wl_\xcb\xe6!:=\xf2\x84\x0e\xe8\x84\xd8*\xf4""0^\xa8\x02\xbc\x92y\xb1l(\xedqx\x8a\xbd\x1bH\xba\xb1?\x88\x99QK\x90T\x02&\xb1""7\xffQ\xcc\xa2mh\xe6\x80\x1a""FS\xb6\x0c\xf1\xf8"
"(sK\x9c\x9c%i\xb0k-{&huy\x7f\x86""5\xe9\n"
"\xd6""0\x1e\xa9\xf2`]\xfe\x91\xe4""aH8s9\x86\xe7\xe0\x14/o\xa4\x19\x98\xac\xf1\xe2\x04\xcf\x8c\xd3\xeeUz1V\xe5\xc1\xb2r\xc0|\xe0\xf4,\xf5\xc0\x81\xcd\xbc\xf5\0\xad""6\xc3)\x0b=n\x86""AQ\xba\xa5""A\x95\x8c\x18\xa6N\x83/Z1\xe6\x87\xb6\xf0\xee""C\x9c\xbf"
"\x1aw\xde[\xc5""5\xf6\xaf\x80\x04\x8b\xb8\xb8\xc8'\x7f\x90$\xbc:\xc5\x93\xe3\xac\xab""3\xb3""0p\xe9""2\xba\x08\xb8\x86\x91\xe2\xacN`z\x98\xa9\xb1x\xfa\x0bY&m\x1a\xca\x16'\x05K+5_\x05\xb8\x84\x1d\xb2R\x0fh\x87\x98""1\x07\xee""a\xd3\x1dqR7\xc4\xd6\x0b\xb1"
"\x08+\xce\xe0\x06.V\x01\xbe\x12s\xf8^\xe2\xc2""5)\xdf\xcdSOyb\x84\xeb\xbd\xe8\xd5""d\x93\xf3\xd7""93\xc7p\xad\x0fr\x19\x9fW\x01\xbe\xc4s9\x80x\xc8\xb3m\xce^\xe1\xc2\xb5\x98""5\xb0\xbe\xce/7\xf8i\x91;\x1b}a\xfa\x19g\xab\0""3\xf8\x1a[\xb1""1\x0f\xd3HJ\xab"
"\xc3oK\xb7v\xda\r4\x13""F\xfb\x8d\xff\x8eo2/\xfa\0p\"\x0b\xd3\xe1\xbc#\x88\xc6\x9a\xff\x95\xf0Q\x9f\xe1\xa3""bG\xf9\xba^\xc2)\x1c_\x95\xb9\x95:\x8a\x8f\x95\x9e\xce\xaa:\xf8\x15\x1f\x88\x99""9\x8d\x9d\n"
"\xd7oI-\x9c\x13\x9f\xcc\x0fq\xa5<aP\xa1\xb5\xf0\x0e\x9e\x12\x1f\xa1\xa7""1je\xc2\xcc\xe1{\x9c\xc6\x8f\xe5\x9d\xe7Jn\xf7\xdf\x96\x7f\0Q\x10\xb1\xe8\x15`\xe8""7\0\0\0\0IEND\xae""B`\x82";

const char* in_png = (const char*) temp_binary_data_13;

//================== nextcue.png ==================
static const unsigned char temp_binary_data_14[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0 \0\0\0 \x08\x06\0\0\0szz\xf4\0\0\0\tpHYs\0\0\r\xd7\0\0\r\xd7\x01""B(\x9bx\0\0\x06\xbbiTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\""
"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl.org/dc/elements/1."
"1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmp:CreatorTool=\"Adobe Photoshop CC (Windows)\" xmp:CreateDate=\"2017-04-23T"
"21:06:06+02:00\" xmp:ModifyDate=\"2019-01-27T14:15:55+01:00\" xmp:MetadataDate=\"2019-01-27T14:15:55+01:00\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpMM:InstanceID=\"xmp.iid:f074bf3d-c8a4-8f47-8e"
"84-1f2784897eab\" xmpMM:DocumentID=\"adobe:docid:photoshop:95e30313-fc38-7b4b-9522-80477eb8b22d\" xmpMM:OriginalDocumentID=\"xmp.did:f3fa921f-8ec6-ea46-a08e-9013d0e125b5\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"created\" stEvt:instanceID=\""
"xmp.iid:f3fa921f-8ec6-ea46-a08e-9013d0e125b5\" stEvt:when=\"2017-04-23T21:06:06+02:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:2c7ab82d-ea2e-e249-ac5b-a61c8fe54910\" stEvt:when"
"=\"2018-06-21T17:49:31+02:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:f074bf3d-c8a4-8f47-8e84-1f2784897eab\" stEvt:when=\"2019-01-27T14:15:55+01:00\" stEvt:"
"softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>r\xb2\x80\xda\0\0\x03+IDATX\x85\xed\x96]HSa\x18\xc7\xff\xefy\xcf\xb4-][EA vQC\x97uQ\t"
"\xab@]j\xda\x87\x94""eR\xd4""E\xda\xc7\x95""D\x10Q\x94\x15I\x11""A\x94^\x96h\x86\x91\x99\x14Y:\xcc(\xcb\n"
"e\x0b)\xa8)\xf6i\xa6\x15\xa4\xcd-\xd3\xb9s\xce\xd3\xc5\x0c\x9a:\x9b\x95\x16\xe4\x1f\xde\x9b\xf3?\x87\xe7\xf7>\xefy\x9e\xf7""aD\x84\xbf)\xe1\xaf""F\x1f\x07\x18\x07\x18\x0e`Y\xa9\r;nX\xb4\0\xa0/\xfc""8\xc8O\xbd\xf2P\xd8YY>\xcd\x9f\x1f\xb0\x88h\xc8""e\xb7"
"\xedk\xa4\xe7k\xc8j\xcd\xaeN-\xbb\xcf\xf5\x05\x1f|\xfc\xd6\x86]nz\x99""BV\xeb\xfe;\xa9""e\x0f\x06\xf9\x81.\xbf\x19\x08\x0b\xfe\x1c\x01O(\xa2\xb5\xcd\x89%\xc6\x02)'\xdc\x12\x0f\0\xec\xac\x13+\xcb\xea\xd9TUw\x10\xdc:DOza.1\x16H\x07\xc2n.\xfe\x95\x04""0"
"\x7f\x8d\xa8\xe3I\x16M\trBV\x82\xc0y\x0f H\xb0""9\"\xaav\xbfZ\xb3\\\x01G\x85\xf1\x0c\xe9T.\x1f\xff\x91\xc3Ps\xbcuYbM\x97""A\xee\xcc\x9c\x1e\x10\x80\xf8\xf3W\x08\xb2\xac\x06WdDOjN\xbe\x1aYH5]\xb3\x8b\x14""b \x12\xfa\xfd\t\xe0\x8a\x82\x85\xba""f\xf3""E"
"\xcd\x07)\xb7=n\x01\xb0\xa9\xe1\x0f""e@\xe5\xf3\x9c""3\x19\xe0}P$5\x08\x0c\xc0\x8f\xdf""3p&\x01""b\x0f\x1a\x1c\x86\xda\x9c\xb7\xc9\xe6kiK\x94\xe1\0""F\\\x86""2q\xc8\x92\xba?\xec@x\x82L\x1c$i0_\xdf\x14s1\xe2\x9c|\xa2\xbax!\0L\xf6S)\xa3\xd2\x07\x14\x12"
" \xf7i\xa1\x11{\xb1""7\xdc""b\xab\xad\xcf)^\x12\xfa\x9a\rU\xae\xa3\xda\x88""d9\x18$\xa9\x11""3\xd9\xbe\xb9\xd4\x98\xaf\xec\x9eqw\xce\x98\x02\0\x80\x02\x06\xd9\x13\x02\rw#{V\xc9\xd3=\x15\xe5""3\xc6\x14`\xa0\x06\xfe""5\x01\x94\xe1\xefI\0\x81\xa9\xba\xd1"
"\xed\t\xc1\xa9""7)\xc6\x93\xabV\xb7\xfb\xfa\xa3(\xce\xdd`b/j:\xa2\n"
"\xd3\xed\xdb\x85S\xedq\x8d\x83\x01G#0S\xc0\x83\x9c\xf8\"M\xc4\xb1\x96\x94y\xe6""E\xd9[\xeb\\3\xe9\xf3\x10\xddq\xc4G\xf0\xd3""F$x\0\xde\x8b\xba\x8e\xa8\xabG\xdf%\xa5U\xa6\x99\x08\0\xfc\xb5\xe6\0""3\xc0\xbc\xbb\x12\xbf\xe2\x93'\x14""e\x1fME\x0eI\r\x81I"
">`\\\xe5\xc2\x17I\x8d#\xaf\xd6""E.2\x1d^\xf7=\xf8p\n"
"\0\x80y/\x1b\xf1+l]\x86\xaa\xb5\x8d\x99,\xaf=.C`\x04\xc6\x14\xaf/\xf6\0\xa2\x1b\xf7:\xe7\x9e\xdf\xd0\xb4""E\xc8m\x8bm\n"
"lc\xc3\x1c""A\xb0\xe0\xdd\x1d\x17]p\xcb\x1a\xe4\xb7%$d\xc5g\xdc\xbe\xff\xd8\x89\x95z;\x9b x\xbc\xbe\xca\tg\x9f\x0eymK\xa3\x0e&mx\x16k\n"
"4\xb4W~3\xd0\xea\xd6""7A\xe5\x82\xcdi\xb8\xb5\xd1\xbeU<\xd4\xb2\xe2""6\0\xd0\x0e-*\xd6\x9b\xe8}\x9f\xd6\x89`\x07\xeeuF]H\xb7oc\xa7\xdb\xcc\xcf""F\x16\xba_\xfe&\x95\xc4KVl\xbf^\xa9%\"\xe8\x86\x98v\x92/\xd7#\xb3\xdc\x12\xfa+S\xd0\x8f\xcb\xefu<V\xfaw\xa7"
"\xe2q\x80\xff\x06\xe0\x1bS}\xda\xa9YIiq\0\0\0\0IEND\xae""B`\x82";

const char* nextcue_png = (const char*) temp_binary_data_14;

//================== out.png ==================
static const unsigned char temp_binary_data_15[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0\x18\0\0\0\x18\x08\x06\0\0\0\xe0w=\xf8\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0\x0b\xaciTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:"
"meta/\" x:xmptk=\"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl."
"org/dc/elements/1.1/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmlns:stRef=\"http://ns.adobe.com/xap/1.0/sType/ResourceRef#\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0"
"/\" xmlns:xmpRights=\"http://ns.adobe.com/xap/1.0/rights/\" xmlns:tiff=\"http://ns.adobe.com/tiff/1.0/\" xmlns:exif=\"http://ns.adobe.com/exif/1.0/\" xmp:CreatorTool=\"Adobe Photoshop CC 2017 (Windows)\" xmp:CreateDate=\"2013-10-04T17:30:37+03:00\" x"
"mp:MetadataDate=\"2019-01-27T14:09:31+01:00\" xmp:ModifyDate=\"2019-01-27T14:09:31+01:00\" dc:format=\"image/png\" xmpMM:InstanceID=\"xmp.iid:2bbda2e4-72b7-064e-bc75-3593afb60418\" xmpMM:DocumentID=\"adobe:docid:photoshop:80723c03-d279-11e6-86fd-f22e"
"a20d4df0\" xmpMM:OriginalDocumentID=\"xmp.did:05bd1fa1-dd19-8d49-a673-1ddd603b57d4\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpRights:Marked=\"False\" tiff:Orientation=\"1\" tiff:XResolution=\"720000/10000\" tiff:YResolu"
"tion=\"720000/10000\" tiff:ResolutionUnit=\"2\" exif:ColorSpace=\"1\" exif:PixelXDimension=\"24\" exif:PixelYDimension=\"24\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"created\" stEvt:instanceID=\"xmp.iid:05bd1fa1-dd19-8d49-a673-1ddd603b57d4\""
" stEvt:when=\"2013-10-04T17:30:37+03:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:97feccd1-2f2d-3245-a56f-9f16d415b29e\" stEvt:when=\"2013-10-04T17:30:44+03:00\" stEvt:softwareA"
"gent=\"Adobe Photoshop CC (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:43d83052-e5e0-3c42-8178-157680ade203\" stEvt:when=\"2017-01-04T13:30:33+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2017 (Windo"
"ws)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"converted\" stEvt:parameters=\"from application/vnd.adobe.photoshop to image/png\"/> <rdf:li stEvt:action=\"derived\" stEvt:parameters=\"converted from application/vnd.adobe.photoshop to image/png\"/"
"> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:bd1e9f9b-1220-eb47-acb3-f57632bb1b8d\" stEvt:when=\"2017-01-04T13:30:33+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2017 (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\""
" stEvt:instanceID=\"xmp.iid:2bbda2e4-72b7-064e-bc75-3593afb60418\" stEvt:when=\"2019-01-27T14:09:31+01:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> <xmpMM:DerivedFrom stRef:instanceI"
"D=\"xmp.iid:43d83052-e5e0-3c42-8178-157680ade203\" stRef:documentID=\"xmp.did:963eb1fc-5696-f44c-bde6-5f8b30db2291\" stRef:originalDocumentID=\"xmp.did:05bd1fa1-dd19-8d49-a673-1ddd603b57d4\"/> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end="
"\"r\"?>j\x08\x83""C\0\0\x03\xc3IDATH\x89\xb5VKlTU\x18\xfe\xfes\xef\xb9\xf7\xce\xb4w:\xd3\xa6XD[!\x86\x84""6\xad""5\x1aL$\x11]4\x06]UC\x82\x89$\x86\xbd\xae\x8cM\\\xe8""F\x8c\xc1\x95+W\xe0\x82-q\x03\x81\x05\xa0\x86\x85\tMMJ\xb1\x06(\xb5\xa0R(t:0g^\xf7q"
"\xce\xef""b:\xb7\x9d\x07jc\xf8V'\xe7\xfc\xe7\xfb\xfe\xe7\xc9!f\xc6\x93\x84\r\0\xe3\xe7>i;xa\xcd\xdf\x1dK\x9a\xaav\xd1""d\xe8\x89\xac\xb6@\x9b\xcf-\rvj\xa6\xe8\x95\xf9\x8c\x1d\xf1\xb1\xb9^5\xd7\xca""1{\xe0\x18""D'\xd5\x11\x95\xf9\xb6\xe2\x8b\xf3""e_\xbc"
"\x17\xa4(\xd7J\x0e\0\xda\x02\x05)\xea\xa9\xf8\xe2\xdd\x8a/\xce\x8d\xa8\xcc\xf1\xc7""F\xb0\xc9\xeb\x9d\x91""C_\x96""3b2\xf0\xc8\xfd\xc7\xd8\x01\x18""A\x08R\xf0\x82\x14=\x1d\xb9\xfc\xfep1\x93\x93!O]\xe9U\x0b\r\x9b$\x82\xf1\xbc\xffl\xe4\xd0\xd1""B\xbfu\xe8"
"\xbf\x90\xb7\"t\xc9)\xf4[\x93\xa1K_\x8d\xe7\xfd\xa1\xb6\x08""B\x97\xbeVYqp\xab\xc4\xadPYk\x92\n"
"\x9a\0\xbc\x93\x08\x8c\xad\xf9\xafV\xbb\xe9\x95XRSM\x04\x08\x06\x0c\x15U\x11\x9a\xb8#aF\xa6\xe1\n"
"\x1b\x06\xf5n\xd4""6(r\xe9\xe5\xb1""B\xe6-\0gm\0\x88<q4\xf4hG\xeb""e\xcd\x06\0""010\x86\xe7\xba\xfb\x9b\xf6""5\x1b\x18""f\xfc\xb8""2\x8f\x95\xda""Cx\x96\x93\x9c\x07\x1em\xb7""b\xfe,\x11\xa8t\xd3\xbe\xd0%\xd9*\xd0\xf0j\xffS\xc3x}\xdbp\xc7\x08*q\x80\x0b"
"\xf7\xae\"b\rk=\x01\x91""Cv\xa5[\xbc\x94\xa4(\x96h#o\xa4\xc8\"\x81\x99\xfc\"TT\x05\x83!\xc9\xc2p\xcf""3\x18\xea\xea\x87""f\x83\xd7\xb6\xed\xc1r\xb5\x80\xe9\xfcM\xa4\xed\x8d\xde\x88\xed:\xb7\r\xd4\xdb\xad\x13\x88\xea\xfb\x97\xf3\x0b\xb8\x9c_\xc0\x80\x97"
"\xc3\x07\xbb\xf6""c\xa8k#]U\x1d\"b\xddv\x97\xd7""9;\x0e\xda""fh6\xb8]^\x85#$&\xb6\x8f\xe2\xc5\xdcN\xfc\xf6\xe8/L\xe7o\x82@\x08L\x8c\xd8h\x08\xea\xec\xa4\xddq\xb7\xe1\xc5\xfa;5\x9a\x1d\xc4\xa1\xa1}\x98\x18\x18\xc5r\xb5\x80\xef~\xff\t=2\x8d\xbd}\xcf\xc3"
"\x13\x12\xd4>\xe8\t\x04\0\x08\xd3\xfe\xe0""13\x88\x08}\xae\x8f#\xbb\xdeH\xc8\xbf\xb9~\x16?\xaf^\x87 \x82""aF\xc4\x1a\x8c\xf6\xfb\xb4\xceY/D\x84(t\x9b\x0b]\x8ak\xd8\xd3\xb3\x03\x1f\xee>\x80\xb1\xec n\xa8\xbb""8\xbe\xf8\x03n\xa8\xbb\x88\xd9\xa0\x14\xd7"
"p\xadx\x07\x17\xef]\xc5R\xe9>\\\xd1\xdc'\xb6\x86N\x04Re3\xcd$\xf6""F\xce""F\xabJa\xa1\x12\x87\x98Y[\xc4\xad\xf2}\xfc\xfa\xf0O\xfc\xb2\xb6\x04""A\x84n\xdb\xc5\xad\xf2\x03\x9c\\\xba\x84kj\x19""5\x13\xc2\xd9$ #\xd6\xa9\x12\xcf%\x02N\x8d?\xd7\x16\x9f\x88"
"\x1c\x1al\x18\xa5-\x17\xf9@\xe1\xe4\xd2%x\x96\x03\x01\x82\xa0z\xdbJ\xb2\xb0\x1a(\xfcQ^E\xdav\xe1\t'\x99\x19\0p\xaa\xbc\xe2V\xcd\x17I\r\xe6r\xea\xa2\x0cy\xda\x8e""a\x1a""F\x06\xf5\x1a""4&\xd4\x80\x93""Aj\xac}\x99\x82""E\xa2\x89\\h\xb0\x0cxf.\xa7\xbeO\x04"
"\0\xc0\t\xf8""c\xbf\xa0O\xb5Uk\x8b\xc8\xac\xe9\xd3n\xc0\x1f%\x82\x8d\xc5l\x9f\xba-C\xfe""4\xf7@\x9f\x92\x01G[%\x96!\xc7\xd9U}\xc6\tyj\xb6O\xddn\xec""7\xcd\xc1\x95^\xb5\x08\xe0\xe0\xc8\xa3\xcc\x89\xd0\xa3""7#\x87zC\x97<~L\x9b\x13\x03""2\xe0@\x86\\pk|a"
"\xde/\x1e""FW\xb3M\xc7I\x9e\xf7\x8bGR\xca\xbc\x9dV\xe6\xb4W\xe1\x92\xa5\xdb\x7f\x06\x96""av\xab\\N\x97\xcc\xf9.e&\xe7\xfd\xe2\xe1\xceN<\xe1_\xc5\xbf\xbe""E\xff\x17\x7f\x03\xa3\xd7\x9d+\x7f\xb5\xf0s\0\0\0\0IEND\xae""B`\x82";

const char* out_png = (const char*) temp_binary_data_15;

//================== padlock.png ==================
static const unsigned char temp_binary_data_16[] =
{ 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,16,0,0,0,16,8,6,0,0,0,31,243,255,97,0,0,0,4,115,66,73,84,8,8,8,8,124,8,100,136,0,0,0,9,112,72,89,115,0,0,0,111,0,0,0,111,1,241,162,220,67,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,155,238,60,26,0,0,1,197,73,68,65,84,56,141,149,147,207,75,212,65,24,198,63,239,204,184,187,186,152,30,190,225,10,9,181,20,88,75,138,244,15,36,68,55,59,247,7,116,242,98,144,215,110,30,2,141,186,117,138,
110,93,186,120,111,97,5,15,130,116,73,232,7,164,132,169,169,184,108,202,254,202,242,251,125,59,140,251,235,187,27,209,11,3,195,51,239,243,60,51,207,204,136,170,18,175,187,139,92,11,29,79,80,106,64,26,216,142,82,60,46,204,82,137,247,186,56,48,189,72,198,
24,22,196,48,155,159,227,16,224,206,18,19,166,206,75,129,251,10,29,142,38,46,96,28,15,140,242,162,65,6,200,207,179,129,240,245,246,51,110,117,245,199,1,81,46,190,157,103,37,142,171,242,70,148,27,255,20,80,161,59,20,64,45,161,244,218,113,175,230,255,41,
81,85,166,159,18,100,3,158,15,166,52,243,173,36,227,70,244,12,64,12,104,8,161,66,24,145,72,37,40,101,210,209,193,126,89,14,138,85,243,176,240,136,162,191,5,203,165,153,73,238,93,30,170,13,70,10,81,4,227,129,119,248,92,108,185,221,28,97,84,32,183,144,
79,148,87,183,205,18,52,4,0,103,209,62,235,231,185,0,62,20,225,184,46,0,12,247,107,19,3,48,34,205,156,186,50,104,52,254,10,225,213,90,146,215,235,73,174,14,183,200,241,106,10,72,27,25,160,242,83,8,210,74,159,133,245,61,249,107,136,77,1,165,211,101,98,
4,234,191,225,52,132,100,236,189,154,54,189,158,215,152,11,96,183,162,92,31,13,153,26,59,35,157,212,142,181,246,114,231,246,110,167,132,29,235,247,41,46,127,242,139,153,11,158,248,229,200,114,101,200,99,239,191,195,143,154,88,212,115,221,249,249,179,
155,71,12,156,148,109,215,110,84,253,248,184,215,194,118,79,204,128,64,22,120,231,0,172,80,221,58,68,247,19,174,35,45,197,191,137,72,149,246,95,95,57,21,181,66,21,224,15,150,241,159,135,159,221,99,248,0,0,0,0,73,69,78,68,174,66,96,130,0,0 };

const char* padlock_png = (const char*) temp_binary_data_16;

//================== pause.png ==================
static const unsigned char temp_binary_data_17[] =
{ 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,32,0,0,0,32,8,3,0,0,0,68,164,138,198,0,0,0,3,115,66,73,84,8,8,8,219,225,79,224,0,0,0,9,112,72,89,115,0,0,0,221,0,0,0,221,1,112,83,162,7,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,119,119,
119,46,105,110,107,115,99,97,112,101,46,111,114,103,155,238,60,26,0,0,0,213,80,76,84,69,255,255,255,36,109,146,64,96,159,53,106,149,51,102,153,51,87,138,46,85,137,57,102,147,43,80,138,43,76,132,52,101,150,48,81,132,48,85,140,46,80,136,54,102,151,54,103,
150,54,102,149,46,79,135,46,78,134,54,103,151,45,77,133,52,97,147,54,103,150,44,76,133,45,75,133,44,73,131,54,103,150,54,102,149,54,103,151,45,75,133,43,71,131,43,72,131,54,103,150,54,103,150,54,103,150,41,65,126,40,64,124,41,66,126,40,61,123,39,59,122,
39,60,123,40,61,123,41,65,126,44,72,131,44,73,131,50,93,143,52,98,147,54,102,149,54,102,150,54,103,150,56,104,149,63,109,151,64,75,111,65,110,152,66,76,110,70,90,118,73,112,138,73,116,153,151,160,159,159,158,155,160,170,164,161,174,166,161,174,167,168,
165,157,168,166,158,169,170,160,171,174,163,242,190,33,242,227,178,255,198,27,255,237,181,14,111,44,112,0,0,0,39,116,82,78,83,0,7,8,24,25,35,39,45,48,54,73,79,102,111,132,151,152,178,184,193,200,200,200,202,204,209,218,219,227,229,236,236,242,243,245,
250,252,253,254,52,59,160,145,0,0,1,39,73,68,65,84,56,203,125,211,107,91,130,48,24,6,224,49,45,139,10,40,148,138,172,152,101,75,83,107,29,177,147,21,226,255,255,73,109,99,192,203,24,60,95,118,177,221,192,78,47,66,69,44,219,243,67,66,66,223,179,45,84,
79,215,141,88,145,200,237,106,195,216,25,177,74,70,14,134,227,157,128,213,18,116,202,241,222,144,25,50,236,21,239,27,199,185,80,223,192,1,107,72,144,205,195,81,143,79,203,229,139,104,227,56,126,84,93,142,92,95,62,255,239,205,230,111,206,219,52,77,223,
238,213,90,196,106,93,86,130,223,201,92,130,215,59,37,92,190,127,17,0,63,148,11,1,232,44,19,145,133,108,6,192,138,114,33,65,46,108,228,105,128,78,50,160,132,135,124,29,80,5,50,225,163,176,17,72,17,34,210,12,232,236,129,145,86,192,5,105,251,133,16,103,
45,147,148,57,175,47,179,10,142,193,70,125,153,192,62,216,234,79,3,24,91,224,176,222,215,235,15,209,153,36,201,115,14,78,224,113,47,166,84,207,205,46,188,48,6,113,168,93,57,93,92,96,253,210,86,197,245,118,253,218,67,113,181,103,42,156,82,92,110,153,75,
79,137,219,35,220,84,188,66,140,251,59,134,250,206,203,127,112,122,0,202,255,31,225,225,147,220,177,251,209,106,0,0,0,0,73,69,78,68,174,66,96,130,0,0 };

const char* pause_png = (const char*) temp_binary_data_17;

//================== play.png ==================
static const unsigned char temp_binary_data_18[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0 \0\0\0 \x08\x06\0\0\0szz\xf4\0\0\0\tpHYs\0\0\0\xec\0\0\0\xec\x01y(q\xbd\0\0\x05\x1ciTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"Ad"
"obe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl.org/dc/elements/1.1/"
"\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmp:CreatorTool=\"Adobe Photoshop CC 2019 (Windows)\" xmp:CreateDate=\"2018-05-"
"13T04:48:26+02:00\" xmp:ModifyDate=\"2019-01-27T14:13:08+01:00\" xmp:MetadataDate=\"2019-01-27T14:13:08+01:00\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpMM:InstanceID=\"xmp.iid:116a9430-8c8a-4747"
"-9881-93b288291091\" xmpMM:DocumentID=\"xmp.did:116a9430-8c8a-4747-9881-93b288291091\" xmpMM:OriginalDocumentID=\"xmp.did:116a9430-8c8a-4747-9881-93b288291091\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"created\" stEvt:instanceID=\"xmp.iid:11"
"6a9430-8c8a-4747-9881-93b288291091\" stEvt:when=\"2018-05-13T04:48:26+02:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\"/> </rdf:Seq> </xmpMM:History> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>?M\xa5\xaa\0\0\x01\xc3"
"IDATX\x85\xed\x97\xbdk\xd5P\x18\xc6\x7f\xcf""9^\xad\x1d\x04\x1d""Eh\xd7\xe2\xe0\xe0\xe8\xc7\xe0\xec""b;\n"
"\x0e:\xe8\xa6""C\x87r\xfd\x03\x04uQ'\x0bup\xd6\xd6\xc1""E\x87\x82_\x7f\x81\x82\xab.\x85N\x85\x0e""E\xc8\xc7\xe3\x90\xb6$\xd5\xc4\xdck\xf0:\xe4\x81\xc0I\xce{\xde\xf7w\xde$O\x88l3I\x85\x89V\xef\x01z\x80\xff\x01\xe0P\xdd""D\xfa\xe5\xc6L@K\xb9\xf2M\x14\x96"
"\x03q\x83,)&c@\xdb)y\x92\xc2\xc9#\x90\x18\xb2\xdf\xec""E\x06\x83\x80pze4\0\xe0\xb6\xe5[B\0g\xc1""C\xe0s\xdb\x9d\xb5U\xed-\x90\xb8\xb4\x7f""b_\xb6\xb3w\xc0\x12""0\xdd""2\xf7U\xe0\x05\xf0\x12\xb8""62\0p\xd0\"\x8f;\xe8\x9e\x83\xde\x02\x17q)B \xc7\xa2W\xfb"
"\x13\xbe\x83Y\0\xe6\r\x8b]\0\xec\xe9\x9c\xe1\x8d\x07<\xe0p8Q\x89\xaa\xae\xc8j\xc6\xad\x01\x1a\xd0<\xe5\xe9\xb8\xc8\xb1\xc1Ge\xbe\x02\xc5\x83""f\xbb\x0cQ\x8f\xf6\xd7\0{)\xcd\x9cS\xad:\xd5\n"
"p\xaa\xb9T\xd7\0%\t\xae\xe7\xca> \xcf{D\x82\x0e\x8d\xc8\xb3HO\x15""43!\x80\xf1\xd4!\x80\xbe""a\xdft\xee\xef\xff\x1c\xc0\xf0,8^\xc0Z\xddu\xce\xd6j\xb2\xe2""f\x15u\xbe*\xfa.\xb0""FR\xb9\xdeZ\xe3u@\xfa\xa1\x9d\xec!\xdb\xc9yG\xad""A\xf1\xf6I*\x03\xb4""Bi"
"\xea@]\x82O\x82\xa1\x12\xde;\xc9\xabQ\xd5\x15\xb1""4\x1et\x01\xb0\xa5\xdc\xf7\x81\xc7""D\xed\xa0R\x84\xc1\xca""8\xe0""BO\x80\x85\"\x91_\x8d\x0c`\xb3.q\xa6@\xd1k\x11\x86\x90""6|\x8e\x7f""1\xa0\xe7\xbbG\xa3\x9a:\xf0H\xd6\xd1\\\xf9&hYh\xe3O\xc9\xc6\x91\xfa"
"\x1f\x93\x1e\xa0\x07\x98""4\xc0O\xfa>\x94|W\xa9(E\0\0\0\0IEND\xae""B`\x82";

const char* play_png = (const char*) temp_binary_data_18;

//================== playing.png ==================
static const unsigned char temp_binary_data_19[] =
{ 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,32,0,0,0,32,8,3,0,0,0,68,164,138,198,0,0,0,4,103,65,77,65,0,0,177,143,11,252,97,5,0,0,0,32,99,72,82,77,0,0,122,38,0,0,128,132,0,0,250,0,0,0,128,232,0,0,117,48,0,0,234,96,0,0,58,152,0,0,23,112,156,186,
81,60,0,0,1,14,80,76,84,69,0,0,0,255,219,36,255,203,28,255,201,27,255,201,26,255,201,27,255,201,27,255,204,31,255,201,27,255,201,27,255,191,32,255,201,27,255,201,27,255,202,32,255,201,27,255,198,28,255,201,27,255,204,36,254,217,97,253,223,126,255,213,
77,254,215,88,253,219,115,255,204,51,254,221,117,255,214,81,253,220,119,255,208,67,254,220,114,255,218,90,254,214,88,253,222,124,254,217,101,255,215,79,254,214,90,254,216,96,253,213,81,255,227,57,255,201,27,184,166,70,124,137,107,199,174,61,70,111,140,
54,103,150,113,132,114,225,186,45,60,106,147,58,105,148,144,147,95,243,195,34,71,111,140,174,161,77,253,200,28,90,121,128,204,176,58,119,135,110,228,188,43,149,149,92,246,196,33,129,144,125,255,203,34,41,63,124,40,62,124,76,96,131,253,220,113,40,60,123,
39,59,122,90,107,131,234,211,133,253,224,133,255,203,35,64,86,129,201,188,133,48,71,127,160,159,133,42,63,124,119,128,133,249,221,133,253,220,114,39,60,123,85,103,131,229,208,133,41,63,123,61,83,129,196,184,134,132,131,103,68,89,130,154,154,134,254,215,
89,0,0,0,37,84,180,234,0,0,0,38,116,82,78,83,0,7,73,152,193,218,243,25,151,242,8,132,245,24,200,45,227,243,206,254,43,183,244,10,250,44,251,65,243,48,182,254,212,45,226,207,114,9,82,166,211,43,0,0,0,1,98,75,71,68,0,136,5,29,72,0,0,0,9,112,72,89,115,0,
0,13,215,0,0,13,215,1,66,40,155,120,0,0,0,7,116,73,77,69,7,226,4,16,21,44,40,36,168,67,185,0,0,1,59,73,68,65,84,56,203,117,147,231,66,131,48,16,128,3,4,202,42,173,86,173,123,27,199,105,221,187,218,22,77,221,117,143,247,127,18,161,172,35,132,239,95,238,
62,72,114,185,35,36,69,81,53,170,27,134,78,53,85,33,69,42,166,197,82,44,179,34,164,109,199,101,57,92,199,198,249,170,199,10,120,213,44,95,171,51,9,245,90,250,189,52,31,24,241,63,108,143,149,224,69,231,112,88,41,206,240,126,201,249,215,55,54,69,193,13,
111,107,38,171,45,128,237,29,193,48,131,250,165,245,105,1,192,238,222,126,78,176,20,162,50,44,0,28,28,30,97,67,37,154,32,0,28,159,32,65,35,180,32,0,156,158,165,65,74,116,137,0,173,243,139,56,168,19,67,38,0,92,182,163,160,129,132,171,107,68,167,219,142,
133,108,139,158,159,210,187,185,229,221,126,180,5,149,8,119,247,60,96,104,80,116,205,68,120,120,228,17,161,161,161,66,69,194,211,243,11,79,24,244,131,66,101,165,14,133,215,183,119,142,24,140,40,232,177,62,124,255,243,139,231,25,197,207,253,253,243,43,
164,121,99,44,215,48,127,98,158,143,11,45,39,26,19,77,177,105,243,198,228,84,177,237,177,49,61,35,27,156,204,152,157,147,143,94,108,52,230,155,101,195,27,26,11,139,75,146,249,78,198,127,121,101,117,45,139,254,3,86,174,144,85,113,87,170,191,0,0,0,37,116,
69,88,116,100,97,116,101,58,99,114,101,97,116,101,0,50,48,49,56,45,48,52,45,49,54,84,50,49,58,52,52,58,52,48,43,48,50,58,48,48,85,236,222,43,0,0,0,37,116,69,88,116,100,97,116,101,58,109,111,100,105,102,121,0,50,48,49,56,45,48,52,45,49,54,84,50,49,58,
52,52,58,52,48,43,48,50,58,48,48,36,177,102,151,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,155,238,60,26,0,0,0,0,73,69,78,68,174,66,96,130,0,0 };

const char* playing_png = (const char*) temp_binary_data_19;

//================== playpause.png ==================
static const unsigned char temp_binary_data_20[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0\x18\0\0\0\x18\x08\x06\0\0\0\xe0w=\xf8\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0:\xa9iTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>\n"
"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"Adobe XMP Core 5.6-c138 79.159824, 2016/09/14-01:09:01        \">\n"
"   <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n"
"      <rdf:Description rdf:about=\"\"\n"
"            xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\"\n"
"            xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\"\n"
"            xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\"\n"
"            xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\"\n"
"            xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
"            xmlns:tiff=\"http://ns.adobe.com/tiff/1.0/\"\n"
"            xmlns:exif=\"http://ns.adobe.com/exif/1.0/\">\n"
"         <xmp:CreatorTool>Adobe Photoshop CC 2017 (Windows)</xmp:CreatorTool>\n"
"         <xmp:CreateDate>2017-04-23T21:05:20+02:00</xmp:CreateDate>\n"
"         <xmp:MetadataDate>2017-04-23T21:05:20+02:00</xmp:MetadataDate>\n"
"         <xmp:ModifyDate>2017-04-23T21:05:20+02:00</xmp:ModifyDate>\n"
"         <xmpMM:InstanceID>xmp.iid:49bd7274-f15f-9b4f-9de0-a5cdd01e7e54</xmpMM:InstanceID>\n"
"         <xmpMM:DocumentID>adobe:docid:photoshop:c85a416d-2857-11e7-9712-974bf39bb726</xmpMM:DocumentID>\n"
"         <xmpMM:OriginalDocumentID>xmp.did:238e9d19-3112-9b49-b462-ce8fbdfd79f1</xmpMM:OriginalDocumentID>\n"
"         <xmpMM:History>\n"
"            <rdf:Seq>\n"
"               <rdf:li rdf:parseType=\"Resource\">\n"
"                  <stEvt:action>created</stEvt:action>\n"
"                  <stEvt:instanceID>xmp.iid:238e9d19-3112-9b49-b462-ce8fbdfd79f1</stEvt:instanceID>\n"
"                  <stEvt:when>2017-04-23T21:05:20+02:00</stEvt:when>\n"
"                  <stEvt:softwareAgent>Adobe Photoshop CC 2017 (Windows)</stEvt:softwareAgent>\n"
"               </rdf:li>\n"
"               <rdf:li rdf:parseType=\"Resource\">\n"
"                  <stEvt:action>saved</stEvt:action>\n"
"                  <stEvt:instanceID>xmp.iid:49bd7274-f15f-9b4f-9de0-a5cdd01e7e54</stEvt:instanceID>\n"
"                  <stEvt:when>2017-04-23T21:05:20+02:00</stEvt:when>\n"
"                  <stEvt:softwareAgent>Adobe Photoshop CC 2017 (Windows)</stEvt:softwareAgent>\n"
"                  <stEvt:changed>/</stEvt:changed>\n"
"               </rdf:li>\n"
"            </rdf:Seq>\n"
"         </xmpMM:History>\n"
"         <photoshop:DocumentAncestors>\n"
"            <rdf:Bag>\n"
"               <rdf:li>DA492819B610466197D7CF5CF85FC565</rdf:li>\n"
"            </rdf:Bag>\n"
"         </photoshop:DocumentAncestors>\n"
"         <photoshop:ColorMode>3</photoshop:ColorMode>\n"
"         <dc:format>image/png</dc:format>\n"
"         <tiff:Orientation>1</tiff:Orientation>\n"
"         <tiff:XResolution>720000/10000</tiff:XResolution>\n"
"         <tiff:YResolution>720000/10000</tiff:YResolution>\n"
"         <tiff:ResolutionUnit>2</tiff:ResolutionUnit>\n"
"         <exif:ColorSpace>65535</exif:ColorSpace>\n"
"         <exif:PixelXDimension>24</exif:PixelXDimension>\n"
"         <exif:PixelYDimension>24</exif:PixelYDimension>\n"
"      </rdf:Description>\n"
"   </rdf:RDF>\n"
"</x:xmpmeta>\n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                                                                                                    \n"
"                            \n"
"<?xpacket end=\"w\"?>\x04\xcc\xba\xd3\0\0\0 cHRM\0\0z%\0\0\x80\x83\0\0\xf9\xff\0\0\x80\xe9\0\0u0\0\0\xea`\0\0:\x98\0\0\x17o\x92_\xc5""F\0\0\x03\xf4IDATx\xda\x9c\x95\xcd\x8b\x14W\x14\xc5\x7f\xef\xa3\xba\xab\xa7\xe8&L\xcf\xb4""F\x92q\xe8\x89""2FP\x18\""
"\xba\x8b\xdb$C\xfe\x02""7\xf9\x07\x0c\xd9""e\x93""E\x16Y%\x08\t\x89\xdb\xa0\x90\x8d""1a\xdc\x88\xe0*\x13p\x17\xcd""B\x10""AdL:\x9d\x96\x99\x9e\xfe\x9a\xfa\xae\xf7^\x16N7cl\xc1\xf6""BAUq\xeb\x9c{\xcf\xbd\xef\x94""8w\xee\x9c\x1c\x0c\x06\x1f\xb7\xdb\xed"
"\xaf*\x95\xca\xbb\x9e\xe7\xc5\xce\xb9\x1c\x10\xcc\x10""B\x08\xe7\x9c\xd3y\x9e\xcfi\xad\xff\xaaV\xab_H)\x7f\xd2""EQ|\xb2\xbd\xbd}\xe9\xe4\xc9\x93\xb5""f\xb3\x89\x10\xa2""b\x8c\xa9\xf0\x1a\xa1\x94\xc2""9\xc7\x93'O\x96\x1e?~|\xb9Z\xad\xd6\xd5h4\xbaz\xfa"
"\xf4\xe9\xb7\xd7\xd6\xd6\xc8\xf3\x9c""8\x8e\x01p\xce\xcdtYk\xc9\xb2\x0c\xdf\xf7Y]]EJYz\xf4\xe8\xd1\x9b\xba^\xaf\xbfQ\xaf\xd7\xd9\xda\xda""booo\xdc\xee\xcc\xd5[kQJ\x91$\t\x83\xc1\x80R\xa9\xc4\xc2\xc2\xc2\xbc\x16""B\xd0\xef\xf7)\x97\xcbx\x9e\xf7:\xca`\xad"
"\xa5T*\xe1\xfb>Y\x96\x11""E\xd1\xa4""3i\x8c\x99\xb0\xcfZ\xb9s\x0e""c\x0cZk|\xdf'MS\xb2,\xc3\xf3<\x8a\xa2 \xcb""2\xe4\xac\x92\x8cs\x8d""1\x14""E\x81\xd6\x9ar\xb9L\x96""edY\x86R\xea\xb9<9\r\xc4\xf3<\xc2""0\xa4\xd5j\x11\xc7""1Z\xeb\xe7\x08\xac\xb5""DQ4\x01"
"\x1d\x0c\x06\xe4y>\x01?\x18z\x1a""A\xb7\xdb""emm\x8d#G\x8ep\xf3\xe6M\x1e>|\xc8\xf2\xf2""2\xbe\xef\xd3\xe9t8z\xf4(\x17.\\ \x08\x02\x8c""1<x\xf0\x80\x1b""7n\xd0h4^\x8d\xa0\xd7\xebQ\xaf\xd7\xb9x\xf1\"\xeb\xeb\xeb\\\xbe\xfc\x03\x9b\x9b\x9b\xfb""C\xcc\t\x82"
"\x80\x13'V\xa9T*\x18\xe3\x18\x0eG\xa4i\x8as\xee\x05\xb9\xa7J\xa4\xb5\xa6\xd7\xef""3\x18\x0e""9u\xea\x14\xdf~\xf7=_\x7fs\x89\xe6\xca\n"
"O\x9fv\x18\x8d""F\x84""aD\xbf?d8\x1c\x10\xc7""1RN\x85\x9a\xde\x81\x10\x02\x9c\xc3""9\xe8\x0f\x9eU\xf7\xd1\x87\x1fp\xfe\xfc\xfb\\\xbbv\x8d?\xef\xdd\xa3\xf5\xcf\xbf\x1c:|\x18\x81\xa3(\n"
"\xd4K\x08\xa6\xbe\x95""B`\xad#IR\x92""4\xa5\xb7\xdb""c\xb7""7 \x98\x9b""cy\xb9I\xb9T&Ib\xf2\xfd!\x1b""c\x10R\xcc\xd0\x81\x14\x18S\x10""E\x11\x8d""F\x03\xbd\xa8\xb8\x7f\xff>\xbf\\\xbf\xce\xc6\xc6\xaf\x1c;\xf6\x0e\xef\x9d""9\x83""1\x05v\x7f]\x85\x98""A"
"\"k\x1d\x9eWbqq\x81N\xa7\xc3\xd5+W\xb8}\xfb\x16""a\x18R\xab\xd5h4\x0e!\x95""Bi\x8d\x94r\x7f=\xdd\xab\x13\xcc\xcdUH\x92\x98\x8d\x8d\r~\xdf\xfc\x8d\xbbw\xff`qa\x91\xc6r\x83\xeen\x97""4\xcdh\xb5\xdax\x9e\x06g\xd9\xe9\xee\xe0io\xea\x81\x15KKK\x7f\xaf\xac"
"\xac\xbc\x15\x04\x01""EQ`\x8c""A)\x85R\x8a^\xaf\x07@\xadVE\x08\x89\xb5\x16!\xc4\xc4\"\xac\xb5\x13\x9b\x1e[\xb5\x10\x02\xa5\x14q\x1c\xb3\xb5\xb5\xd5\xd2\xff""7-!\x04\xbe\xef""c\xad%\x08\x82\xfd\xaa\xc4\x04\xec\xa0\x95\x8f+v\xce\xbd\xd4Z\xf4\xc1\x8f\x94"
"R\x94\xcb""e\xac\xb5$I2Y\xd9\x83\0""c\xe0\x83\xf6\xf1""2\x12!\xc4\xf3""3\x90RN\\p\xfc\xfc\xba!\xa5|F\x90""e\x19""EQ\xe0y\x1e\xe3\xfbi\xa6""5\x0b\xb0\xd6\x9a\xe1pH\x9a\xa6\xe8$I\x92n\xb7\x8b\xef\xfbh\xad\xb1\xd6\x92\xe7""9Z\xeb\x17\xe4\x99\xf6\xa3\x19"
"K;\xce\x15""B\x10\xc7""1;;;\x84""a\x98\xe8\xb3g\xcf\xfe|\xe7\xce\x9d\xcf\xf6\xf6\xf6\xfc""F\xa3""A\xa9TBkM\x14""E\xcfN\xe8\x94\xd5\x1b\x93z\x9e\x87\xd6\x9a""0\x0c""1\xc6Ld\xd9\xdd\xdd""e{{;;~\xfc\xf8--\xa5\xfcr~~^\xe6y\xfei\xbb\xdd.W\xab\xd5\xb0V\xab"
"\xb9~\xbfO\x18\x86\x08!^ \x19\x13\xd7\xebu|\xdf\xa7\xdb\xed\x12""E\x11Zk\xf2<\x9fK\xd3\xd4""6\x9b\xcd\x1f\xd7\xd7\xd7?\xffo\0\x05\x1a""4\x8d\x01%\xca%\0\0\0\0IEND\xae""B`\x82";

const char* playpause_png = (const char*) temp_binary_data_20;

//================== prevcue.png ==================
static const unsigned char temp_binary_data_21[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0 \0\0\0 \x08\x06\0\0\0szz\xf4\0\0\0\tpHYs\0\0\r\xd7\0\0\r\xd7\x01""B(\x9bx\0\0\x06\xbbiTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\""
"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl.org/dc/elements/1."
"1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmp:CreatorTool=\"Adobe Photoshop CC (Windows)\" xmp:CreateDate=\"2017-04-23T"
"21:06:06+02:00\" xmp:ModifyDate=\"2019-01-27T14:16:08+01:00\" xmp:MetadataDate=\"2019-01-27T14:16:08+01:00\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpMM:InstanceID=\"xmp.iid:c871c922-f353-6e44-a7"
"31-17b674c9bd61\" xmpMM:DocumentID=\"adobe:docid:photoshop:1e819083-c98b-6447-81ad-8246dea5e281\" xmpMM:OriginalDocumentID=\"xmp.did:f3fa921f-8ec6-ea46-a08e-9013d0e125b5\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"created\" stEvt:instanceID=\""
"xmp.iid:f3fa921f-8ec6-ea46-a08e-9013d0e125b5\" stEvt:when=\"2017-04-23T21:06:06+02:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:2c7ab82d-ea2e-e249-ac5b-a61c8fe54910\" stEvt:when"
"=\"2018-06-21T17:49:31+02:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\" stEvt:changed=\"/\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:c871c922-f353-6e44-a731-17b674c9bd61\" stEvt:when=\"2019-01-27T14:16:08+01:00\" stEvt:"
"softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>\xeb\xb7\x1a\xf3\0\0\x02\xd3IDATX\x85\xed\x97KH\x94Q\x18\x86\x9f\xf3\x9f\x7f\xe6W\x9b"
"\x19\xad\x8dT\x9aZ&3f\xab""2\x8a\x16""BEt\x81\x16""a\x84""EYH\xad\xba\x10\xd8\"*\xda\x04\xd1\xa2]\x10$\xe2\xd0\x85\xee\x0b)(/%\x18\x88\xa5\x18\x88\x8d\x16\x15I\x10\x19\x92\xce\x8c\x8d\x8e\xff\xad\x85""ch98\x92\x97""E\xbe\xbb\xc3\x0b\xdf\xf7\x9c\xf7||"
"p\x84m\xdb\xcc\xa5\x94""9\xed>\x0f""0\x0f\0\xa8""c\x0f\xe5O\xaaIU\x87h\x0egS\x9eQGSx\x05M\xa1\x1c\xce""e\xd4P\xdb\xef\xa3%\xbc\x8c\xd3\x99\xf5""4\x06s9\xb9\xf4\x05o\x06""2\xb8\xf1\xbd\xd0}.\xe3y\xf8\xf8\xc7""b\0.d=\xe5U8\x9b\x17}+9\x93UC\xc5\xb7\r\x9e"
"S\x8b_\x86|k/M\x08\xf0\xaf\t\xdc\xac,\xb8\x16\x02\x82\x13x\x12\xa8\xbd\xee\xab\x08\x02\x9d\xf1\n"
"\xa8\xf1\x8cI\xb4\n"
"\xe8(Z\xd4\x01\xd1""4\x16;C\x1e@\0\xa3Ke\xf3\x9d\xfc\xcazMF@w\x93\xa9\xf5y\xe3\x15\x9aj\x02\x02\xf0\x9f\xcf\xa9\xee\xf0""8\x06""0u\x0f\0""C\x96\x03\xc0\xf6\xe7\xdd\x06xz,\xf3Y\xbd\xa6\x0c""c\x1an\0\xa2V\xfc{N%\x01\xef]\xaf\xbf\xd3\xa5\xf5\x81\x91\x8c"
"iK\xc0\xc6\xb6\x15,[P\x9a\xfe\xba\n"
"8T\x98\xfa\x1e\x8c\x94\xdf\xfe""dJ\x04@\0\x0f/,\x7f\xb4\x1b""3\tSw\x8f+l\xd9*i\xea {\xd2\x9b\x0f""a:1\x8d\x94\x98\x93\xd8\x8a\x9f\x0c`\xf5=oU\xbbK\xfb\x01\xba\x0b\xd3V&(lc\x03\xa6\x91\x9cP\xc3?\x15o\x06\x04Py6\xebq\xbbK\xfd\x89""9\xec\x89""5\x9f~M\x94"
"\x80\xef~~E`\x81""c\0[O\xc1""B\xccH\xe3x\0K\xce\xe7\xde\t`\xb8""0u\x17\x89\xbe\xe3\xbfh\\\xae""3{\xd7\x04\0\x80\xaf\x17?\x94\x14""DL\r\xe9\x18@\x99\xed\x04""bz\xbb""7pDi\xfc\x91\x7fK\xa8\x83H\x19\x9du\0\x18y\xfc\x03\x97\xbbw\x14""F\x8c$\xa4""3\x84\"\xac"
"Y\x05\x18U\xeb\xbe\xae\xc3\xb2\xad\xcf\xdb(\xd4\x08R\x98\xfc=)\x02\x01Hu0\xe6O/\0\x80\x05\x14]\xfe\xbckM\xd4r \x1d""a\xe4\x98""4\x14""a\xd0o$\xf3\xa0g\xbd\xbfWw#\xd5H\xccOl\xa4\xa7\xb2]\xda\xf6\xbd;\xa8\xb6\xf6\xe7""5\xa0""F\x90r\x08\x10\x08""a\xa1\x08"
"\x1b\xe0""0 Z\x82y\xcf""F\xfc\xc1\x84 \xa6\xba\xdeL`\xd3\x95\xee\x9d\x1b\xa3\x96\x86t\x84\x01HRt\xfc=\xeb""D\xe9\xfb\xfd\0\xdb\xaf~\xd9\xb6%j9\x91\xea\x88\xaf)\xc6\xb4\x01\x8c\xaa\xa9$P\xa6\xb6\x04s\x1b\xd0\xfa\xe9\xd5\x17\x0c""3~k=/\t\x94\xa9-\xa1\xbc"
":\x1c""a\xbe""D\x17vM7\0\xc4\xd2""8\xd1Y\x96~\xfcSq\xb2S1q*\xe6\x9f\xfe\xd6\xa3\x9dGR\x01_\xbc\"b\xfeg4\x0f\xf0\xdf\x03\xfc\x02\x08\xe3\xfc,\xa1\xf5\xdd\xd8\0\0\0\0IEND\xae""B`\x82";

const char* prevcue_png = (const char*) temp_binary_data_21;

//================== smallstripe.png ==================
static const unsigned char temp_binary_data_22[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0\x0b\0\0\0\x16\x08\x02\0\0\0\x99\x86\xf1""4\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0\x05\xebiTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adob"
"e:ns:meta/\" x:xmptk=\"Adobe XMP Core 5.6-c142 79.160924, 2017/07/13-01:06:39        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://"
"purl.org/dc/elements/1.1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmp:CreatorTool=\"Adobe Photoshop CC (Windows)\" xmp:C"
"reateDate=\"2018-05-12T23:57:17-04:00\" xmp:ModifyDate=\"2018-05-13T00:41:32-04:00\" xmp:MetadataDate=\"2018-05-13T00:41:32-04:00\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpMM:InstanceID=\"xmp.ii"
"d:945456c9-40d1-b140-a3e4-19f3a20133ef\" xmpMM:DocumentID=\"adobe:docid:photoshop:eb1bc8ac-533c-144c-9b16-0ce08a867c7a\" xmpMM:OriginalDocumentID=\"xmp.did:dc6949d3-d464-be4e-9533-44bb10aa234d\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"creat"
"ed\" stEvt:instanceID=\"xmp.iid:dc6949d3-d464-be4e-9533-44bb10aa234d\" stEvt:when=\"2018-05-12T23:57:17-04:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:945456c9-40d1-b140-a3e4-1"
"9f3a20133ef\" stEvt:when=\"2018-05-13T00:41:32-04:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>Y\xb3H,\0\0\x01""bIDAT(\x91u"
"\x92\xb1\xce\x01""A\x14\x85\xcf\xdc\x8c\xd8\xa5\xd9""bu<\x89R)\x12\xa2\x92)\xb6\xd0\x88\x08\x95G\xd0\x89h\x14\xca}\x01\r\xd1\x11\x89\x07Ph\xd5+h\x14\xc4\x9aHV\xb1""2{\xf7O\xfe\xdb\xcc\xfdrNNf\xee\x1dQ\xab\xd5\x9e\xcf'\0\0""a\x18\x0e\x87\xc3z\xbd\x0eV"
"\xf2v\xbbq.\x97\xcb\x1c\xef\xf7;\xe5r9\xc3\x9e\xe7\x15\n"
"\x05\xeeX\xadV\x94\xa4I\xe9y\x1e\x97\xaf\xd7\xab\xef\xfb\x89""C)e\xdb""6w\xcc\xe7\xf3\xc7\xe3""AD\x04\xc0q\x9cv\xbb\xcd\xe5\xcb\xe5\xb2^\xaf\x89\x88\xb4\xd6\0z\xbd^&\x93\xe1\x8e\xe9t\n"
"\xc0\xb2,\xd2Z[\x96U\xadV\xb9\x1c\x04\xc1n\xb7\x03 \x84\xa0\xf8\tH\xd7l6\x8b\x9b(\x8a\xa8X,\xb6Z-.\x9f\xcf\xe7\xedvk\x90\xfa\xfd~6\x9b\xe5\x8e\xc9""db\xfa""0\x0c""E\x14""E\\>\x1c\x0e\xddn\xd7\xa0\xe3""8\x84t-\x16\x0b\x8e\x8d""F#\xe5x\xbd^\xfb\xfd\xde"
"\xa0\x94R)\x95r\xf8\xbe\xff~\xbf\r*\xa5\xf2\xf9|r\x0f\xadu\xa5R\xf9|>1\n"
"!6\x9b\x8dm\xdbI\xc6x<62\x80\xc1`\x10\xaf\xe9\x97\x11\x04""A\xb3\xd9""4\xb2\xeb\xba\xcb\xe5\xf2""7\x8f\xf8""0C\x8c\xab\xd3\xe9\x98\x9e\xe2\0>D\xd7u\xf9\x9a\x08\xc0h4\xfa/\0\x80<\x9dN\xc7\xe3\xd1\xfc\xc5R\xa9\xf4g\xcf_\x82\xfd\x89\"\x99\x91\xa5\xe2\0\0"
"\0\0IEND\xae""B`\x82";

const char* smallstripe_png = (const char*) temp_binary_data_22;

//================== snap.png ==================
static const unsigned char temp_binary_data_23[] =
{ 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,32,0,0,0,32,8,6,0,0,0,115,122,122,244,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,154,156,24,0,0,6,182,105,84,88,116,88,77,76,58,99,111,109,46,97,100,111,98,101,46,120,109,112,0,0,0,0,0,60,63,120,
112,97,99,107,101,116,32,98,101,103,105,110,61,34,239,187,191,34,32,105,100,61,34,87,53,77,48,77,112,67,101,104,105,72,122,114,101,83,122,78,84,99,122,107,99,57,100,34,63,62,32,60,120,58,120,109,112,109,101,116,97,32,120,109,108,110,115,58,120,61,34,
97,100,111,98,101,58,110,115,58,109,101,116,97,47,34,32,120,58,120,109,112,116,107,61,34,65,100,111,98,101,32,88,77,80,32,67,111,114,101,32,53,46,54,45,99,49,52,50,32,55,57,46,49,54,48,57,50,52,44,32,50,48,49,55,47,48,55,47,49,51,45,48,49,58,48,54,58,
51,57,32,32,32,32,32,32,32,32,34,62,32,60,114,100,102,58,82,68,70,32,120,109,108,110,115,58,114,100,102,61,34,104,116,116,112,58,47,47,119,119,119,46,119,51,46,111,114,103,47,49,57,57,57,47,48,50,47,50,50,45,114,100,102,45,115,121,110,116,97,120,45,110,
115,35,34,62,32,60,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,32,114,100,102,58,97,98,111,117,116,61,34,34,32,120,109,108,110,115,58,120,109,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,
49,46,48,47,34,32,120,109,108,110,115,58,120,109,112,77,77,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,109,109,47,34,32,120,109,108,110,115,58,115,116,69,118,116,61,34,104,116,116,112,58,47,47,
110,115,46,97,100,111,98,101,46,99,111,109,47,120,97,112,47,49,46,48,47,115,84,121,112,101,47,82,101,115,111,117,114,99,101,69,118,101,110,116,35,34,32,120,109,108,110,115,58,100,99,61,34,104,116,116,112,58,47,47,112,117,114,108,46,111,114,103,47,100,
99,47,101,108,101,109,101,110,116,115,47,49,46,49,47,34,32,120,109,108,110,115,58,112,104,111,116,111,115,104,111,112,61,34,104,116,116,112,58,47,47,110,115,46,97,100,111,98,101,46,99,111,109,47,112,104,111,116,111,115,104,111,112,47,49,46,48,47,34,32,
120,109,112,58,67,114,101,97,116,111,114,84,111,111,108,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,87,105,110,100,111,119,115,41,34,32,120,109,112,58,67,114,101,97,116,101,68,97,116,101,61,34,50,48,49,55,45,49,49,45,
50,49,84,48,48,58,50,49,58,49,48,43,48,49,58,48,48,34,32,120,109,112,58,77,101,116,97,100,97,116,97,68,97,116,101,61,34,50,48,49,56,45,48,49,45,48,52,84,49,53,58,50,51,58,49,51,43,48,49,58,48,48,34,32,120,109,112,58,77,111,100,105,102,121,68,97,116,101,
61,34,50,48,49,56,45,48,49,45,48,52,84,49,53,58,50,51,58,49,51,43,48,49,58,48,48,34,32,120,109,112,77,77,58,73,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,98,102,102,53,99,51,48,101,45,101,100,101,102,45,55,55,52,52,45,56,99,56,
54,45,52,57,99,53,49,51,57,50,97,97,101,101,34,32,120,109,112,77,77,58,68,111,99,117,109,101,110,116,73,68,61,34,97,100,111,98,101,58,100,111,99,105,100,58,112,104,111,116,111,115,104,111,112,58,98,52,98,51,48,100,102,48,45,51,56,54,54,45,53,97,52,102,
45,98,97,101,57,45,99,49,100,102,51,52,100,51,100,48,54,50,34,32,120,109,112,77,77,58,79,114,105,103,105,110,97,108,68,111,99,117,109,101,110,116,73,68,61,34,120,109,112,46,100,105,100,58,101,53,49,56,56,51,57,56,45,102,57,99,56,45,51,97,52,98,45,97,
56,101,49,45,52,49,55,57,53,55,98,102,97,97,48,49,34,32,100,99,58,102,111,114,109,97,116,61,34,105,109,97,103,101,47,112,110,103,34,32,112,104,111,116,111,115,104,111,112,58,67,111,108,111,114,77,111,100,101,61,34,51,34,32,112,104,111,116,111,115,104,
111,112,58,73,67,67,80,114,111,102,105,108,101,61,34,115,82,71,66,32,73,69,67,54,49,57,54,54,45,50,46,49,34,62,32,60,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,114,100,102,58,83,101,113,62,32,60,114,100,102,58,108,105,32,115,116,69,118,
116,58,97,99,116,105,111,110,61,34,99,114,101,97,116,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,101,53,49,56,56,51,57,56,45,102,57,99,56,45,51,97,52,98,45,97,56,101,49,45,52,49,55,57,53,
55,98,102,97,97,48,49,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,50,49,84,48,48,58,50,49,58,49,48,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,
80,104,111,116,111,115,104,111,112,32,67,67,32,40,87,105,110,100,111,119,115,41,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,
68,61,34,120,109,112,46,105,105,100,58,48,57,99,48,52,55,51,100,45,97,57,48,54,45,52,55,52,97,45,98,54,99,49,45,51,53,100,99,51,57,97,56,48,51,97,49,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,55,45,49,49,45,50,49,84,48,48,58,50,49,58,
49,48,43,48,49,58,48,48,34,32,115,116,69,118,116,58,115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,87,105,110,100,111,119,115,41,34,32,115,116,69,118,116,58,99,104,97,110,
103,101,100,61,34,47,34,47,62,32,60,114,100,102,58,108,105,32,115,116,69,118,116,58,97,99,116,105,111,110,61,34,115,97,118,101,100,34,32,115,116,69,118,116,58,105,110,115,116,97,110,99,101,73,68,61,34,120,109,112,46,105,105,100,58,98,102,102,53,99,51,
48,101,45,101,100,101,102,45,55,55,52,52,45,56,99,56,54,45,52,57,99,53,49,51,57,50,97,97,101,101,34,32,115,116,69,118,116,58,119,104,101,110,61,34,50,48,49,56,45,48,49,45,48,52,84,49,53,58,50,51,58,49,51,43,48,49,58,48,48,34,32,115,116,69,118,116,58,
115,111,102,116,119,97,114,101,65,103,101,110,116,61,34,65,100,111,98,101,32,80,104,111,116,111,115,104,111,112,32,67,67,32,40,87,105,110,100,111,119,115,41,34,32,115,116,69,118,116,58,99,104,97,110,103,101,100,61,34,47,34,47,62,32,60,47,114,100,102,
58,83,101,113,62,32,60,47,120,109,112,77,77,58,72,105,115,116,111,114,121,62,32,60,47,114,100,102,58,68,101,115,99,114,105,112,116,105,111,110,62,32,60,47,114,100,102,58,82,68,70,62,32,60,47,120,58,120,109,112,109,101,116,97,62,32,60,63,120,112,97,99,
107,101,116,32,101,110,100,61,34,114,34,63,62,203,67,21,126,0,0,7,49,73,68,65,84,88,9,197,193,77,140,220,117,25,192,241,239,243,252,95,102,102,103,167,187,219,238,118,251,66,91,104,11,182,242,102,161,193,166,74,32,8,74,34,193,144,24,15,198,104,194,69,
163,23,77,12,71,19,229,4,196,151,11,122,34,36,94,240,224,65,98,16,21,193,131,111,28,138,64,237,11,47,125,217,150,118,119,103,59,51,187,51,179,51,243,127,249,253,127,143,140,80,28,234,22,170,30,248,124,196,204,248,40,41,31,49,229,35,22,190,222,60,199,
40,17,161,159,12,88,90,105,16,71,17,73,154,18,71,17,174,40,214,245,178,193,221,169,119,159,115,86,220,92,152,223,236,177,138,32,185,138,52,34,9,94,43,7,241,31,43,97,252,108,28,198,103,124,81,16,134,1,222,27,99,149,10,51,235,55,224,189,103,232,218,137,
173,92,20,242,1,84,132,183,141,181,211,222,183,186,249,224,235,206,138,93,188,75,16,134,12,195,153,109,113,86,220,212,47,178,47,5,78,219,213,160,244,84,45,170,60,22,75,116,202,83,240,65,148,203,80,17,18,151,223,214,72,59,127,110,165,221,71,157,21,187,
4,65,16,4,97,148,0,130,160,34,152,217,68,39,31,124,99,113,176,242,98,59,237,63,168,34,124,16,101,13,129,42,171,217,224,129,249,94,243,15,153,119,251,84,20,65,184,82,42,130,199,102,46,36,43,79,92,232,183,31,81,85,4,97,45,42,10,162,32,10,162,16,4,74,39,
237,221,223,204,186,191,240,230,107,34,194,255,66,0,65,88,201,123,15,53,250,237,199,204,27,24,96,188,143,122,243,120,243,120,243,12,117,147,222,238,147,205,115,79,10,18,11,194,255,43,212,128,249,118,227,187,111,53,23,191,90,184,130,44,203,24,165,105,
150,145,102,25,89,150,145,164,169,156,88,122,235,241,204,229,235,69,132,181,20,230,201,125,129,97,92,41,21,225,204,242,226,143,58,131,213,171,92,225,24,165,188,75,52,96,101,176,122,111,59,89,253,108,160,1,107,41,188,103,99,117,130,221,235,183,32,8,222,
140,43,33,34,228,69,190,97,169,183,242,80,16,70,140,210,36,75,73,178,148,36,29,176,216,109,124,135,203,48,64,68,248,228,246,235,184,107,247,62,62,127,221,126,38,163,10,30,227,74,168,42,205,94,251,43,157,94,103,150,17,202,219,68,148,36,207,118,247,179,
228,14,21,101,45,206,23,236,218,188,131,217,241,77,128,167,234,3,106,89,68,238,10,84,132,33,111,70,97,158,181,8,130,243,110,170,147,244,238,99,132,230,133,195,21,142,110,58,56,88,152,197,172,193,68,9,139,130,217,23,158,165,255,198,223,241,24,231,151,
26,28,105,44,50,223,104,227,188,103,168,18,197,76,150,171,228,133,195,155,177,150,213,108,112,15,35,66,239,61,136,145,186,244,70,46,195,71,17,91,94,63,130,190,240,12,103,95,124,30,183,109,63,39,15,220,193,82,214,71,188,49,127,161,205,244,84,149,79,237,
216,203,53,235,183,112,162,121,142,151,207,159,34,41,50,4,225,34,17,37,113,217,30,70,104,146,38,12,146,132,212,229,219,68,248,15,166,74,220,91,101,203,171,47,33,213,42,46,23,142,253,234,119,28,62,125,28,9,2,84,133,118,127,64,28,214,184,118,102,59,81,
16,112,109,109,150,89,171,82,120,67,132,247,8,224,188,159,102,132,182,186,29,90,221,54,153,203,199,5,225,82,62,140,216,124,244,21,170,173,38,68,33,141,185,54,139,55,222,64,111,211,52,90,20,12,169,192,13,75,243,88,103,25,136,56,191,184,196,155,141,37,
234,173,46,32,136,240,30,195,202,140,8,39,170,227,168,8,173,116,53,113,62,67,16,46,178,32,160,178,178,204,166,99,135,177,82,76,210,73,89,232,43,205,219,247,163,222,51,148,135,33,219,91,117,38,127,251,28,167,255,244,27,162,91,239,225,212,142,61,156,29,
180,241,133,103,177,217,97,211,134,117,136,24,102,32,144,51,66,171,229,10,213,242,24,165,48,94,48,227,125,124,16,48,125,234,13,202,171,93,76,149,11,115,43,212,111,185,145,116,118,3,226,10,12,33,40,60,55,159,58,70,16,69,164,203,45,94,253,225,227,188,116,
248,16,62,8,8,84,105,175,14,168,183,186,128,48,20,72,176,204,8,21,17,68,132,56,12,143,114,41,111,84,27,75,160,138,75,114,234,65,149,230,193,125,104,158,51,228,194,144,157,139,103,217,212,172,227,227,144,118,125,192,194,236,22,154,27,106,4,133,103,72,
85,88,233,246,89,106,117,49,140,177,82,249,36,35,52,12,34,130,32,164,26,143,253,77,69,60,163,4,44,8,48,51,34,60,178,103,27,189,169,73,188,129,11,66,34,151,115,211,201,163,160,74,158,20,44,212,19,150,62,115,16,74,49,152,113,145,170,176,220,237,83,111,
117,41,50,255,60,35,52,80,65,5,170,113,233,72,57,140,95,246,102,252,155,176,178,117,27,67,22,71,92,31,116,56,112,244,16,235,250,171,140,15,122,28,56,126,136,245,221,101,136,66,154,103,219,44,94,125,13,189,221,219,209,44,231,82,170,66,103,53,25,28,59,
49,247,52,35,194,40,142,25,10,68,139,153,241,169,159,206,181,22,158,64,132,33,117,57,141,157,215,49,115,228,31,76,47,157,167,36,194,45,111,30,230,227,103,222,192,68,40,103,41,86,138,24,116,82,234,77,71,227,11,183,129,177,38,51,163,164,193,175,203,177,
158,102,132,170,40,42,202,208,244,248,196,83,213,184,124,196,155,103,72,204,176,40,228,205,3,119,50,223,11,41,58,3,44,142,40,81,80,54,135,197,33,189,229,148,51,175,212,57,127,224,22,6,219,55,163,121,206,154,132,124,247,150,109,143,236,221,185,139,81,
114,188,113,6,51,99,40,80,101,165,191,250,233,35,139,167,94,0,34,225,29,22,199,248,185,121,194,167,127,207,85,245,115,212,66,143,247,176,218,78,105,246,161,126,240,86,234,119,31,68,188,7,51,46,149,59,199,158,171,182,63,188,119,235,213,223,115,190,224,
246,29,123,185,72,142,215,231,208,40,4,51,204,140,80,3,230,90,11,223,62,221,156,255,177,170,34,188,67,226,136,213,36,167,245,210,113,198,95,59,73,220,237,147,79,214,232,238,221,201,96,235,44,154,59,48,227,82,174,112,108,158,154,126,230,206,235,247,61,
160,170,185,55,227,19,27,183,115,81,216,62,83,39,170,141,81,158,26,71,131,128,188,40,152,169,76,254,164,21,175,212,186,110,240,3,67,16,192,178,156,90,168,200,173,123,56,119,205,86,204,123,80,69,92,129,102,57,107,41,188,103,98,108,252,185,187,110,218,
255,229,169,234,120,238,124,193,165,212,138,130,164,209,102,101,110,17,151,229,136,42,133,121,166,226,241,135,55,86,38,31,84,164,233,205,24,242,222,51,30,42,219,106,21,194,220,161,131,4,117,142,75,121,51,12,88,87,25,251,217,214,169,233,251,43,165,114,
199,99,32,2,34,140,82,68,144,64,113,73,202,202,220,2,69,154,35,170,120,140,137,210,216,147,155,42,147,7,106,113,229,231,64,98,24,133,247,84,43,49,155,167,215,161,42,152,25,67,102,134,55,195,204,168,196,165,191,206,212,38,238,157,169,77,124,211,140,196,
155,231,114,66,222,37,170,184,36,99,249,204,2,165,77,147,136,8,222,140,72,195,19,51,149,201,175,173,139,178,71,187,105,255,139,169,119,247,20,190,248,88,173,82,158,96,3,209,66,179,99,64,47,10,194,179,113,24,254,165,20,197,191,156,168,84,159,115,190,48,
111,198,135,9,25,33,129,146,15,82,242,115,23,208,169,18,8,24,134,97,68,26,30,173,5,149,163,227,145,125,191,240,126,218,227,55,86,42,97,165,178,177,236,218,253,180,85,142,163,5,65,156,243,5,134,97,102,136,8,31,38,52,179,251,84,117,76,68,94,3,14,139,10,
228,158,112,37,195,54,4,32,252,139,97,24,198,144,138,52,2,194,70,102,25,113,24,17,106,206,144,55,195,204,248,111,252,19,142,97,179,28,126,185,64,78,0,0,0,0,73,69,78,68,174,66,96,130,0,0 };

const char* snap_png = (const char*) temp_binary_data_23;

//================== stop.png ==================
static const unsigned char temp_binary_data_24[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0 \0\0\0 \x08\x06\0\0\0szz\xf4\0\0\0\tpHYs\0\0\r\xd7\0\0\r\xd7\x01""B(\x9bx\0\0\x05\x1ciTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\""
"Adobe XMP Core 5.6-c145 79.163499, 2018/08/13-16:40:22        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl.org/dc/elements/1."
"1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmp:CreatorTool=\"Adobe Photoshop CC 2019 (Windows)\" xmp:CreateDate=\"2017-0"
"4-23T21:03:24+02:00\" xmp:ModifyDate=\"2019-01-27T14:13:57+01:00\" xmp:MetadataDate=\"2019-01-27T14:13:57+01:00\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpMM:InstanceID=\"xmp.iid:b76d7782-8831-9f"
"42-a897-a900903ff943\" xmpMM:DocumentID=\"xmp.did:b76d7782-8831-9f42-a897-a900903ff943\" xmpMM:OriginalDocumentID=\"xmp.did:b76d7782-8831-9f42-a897-a900903ff943\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"created\" stEvt:instanceID=\"xmp.iid:"
"b76d7782-8831-9f42-a897-a900903ff943\" stEvt:when=\"2017-04-23T21:03:24+02:00\" stEvt:softwareAgent=\"Adobe Photoshop CC 2019 (Windows)\"/> </rdf:Seq> </xmpMM:History> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>\xc8\xa5\xc9""E\0\0"
"\x02""DIDATX\x85\xed\xd7""Ak\x13""A\x14\x07\xf0\xff{3\x9b\xddM\xd2\xdd""D\xa9UT\x10*b\x0bV\x10\xec\xc1\xa3'\xf1\xd0\x93""7\xc1\x8b\xe0\xd9/\xe0G\xd0\x9b \xde\xc5\xb3\xe0\xd1\x83_\xc0""B\n"
"\xc5\"\x16-\xa5\x16\xdb&mM\x93\xecnf\xe6y\xa8\x8dUWX\xa4\x12\xc1<\x98\xcb\xbe""a\xe7\xc7\xdb\x19\xe6-\x89\x08\x86\x19<\xd4\xd5G\x80\x7f\x01\xa0\xf3\x1e>\xbe""5\x87\x8e\xd3|\xa3\xb6\xaaNy=\xbe\xbfrM\x7fH\"\x8aT6\x98# \xe7\xb1\tj\xbaw\xc6#\xeb;\x90=\xc8"
"1De\xa2\xd2\xb5""4\xfa""47\xbe\xd8\xbey\xfc\xad\xfd""b\x02""3\xfb\xf4}1@\xc0\x96[\xc6\x9fi\xf6\xc3;S\xe5\xd6\xd5\x07\xa7\xdfp\xe2\x14""4}?1\x02\x12\xd6)\xebp+ eY\x04\x83$\x01\xe4\0\x97""A\xa7i\xcf""7\xdb\xfd\xea\xcb\x9a\x9f=\x04`\n"
"\x01\x16\xbbu\x9e\x1d\xdb\xb8}6\xdc\xb9\xeb\x91\x8b.\x8e}\x06H\x80\x1fN,\x01:\x01\xaaM@\xe1\xa7\xdc\xa0\x14H\x92\x12\x16""6N\x9aW\x1f\xe3GS\xf9S~\x8d\xf9\xee\xb8\x9a\t[\xd7\xcfW7#\x08\x03\xc6\x07\xfa\x01`rF_\x03}\xe4\x8f\x14\x08\xaa\x19\"\xb5\xed\xbf"
"x\x17\xab\xbc\xb5r\x01>Y\xed@)\xdc\x11\xecQ\x07""8\x10\x95\xb5+\x17\x06\x10@\xc8/\xea\x9f""FH\x84\xc9\xc2\x80\xbf\x10%\0\x13\xc3\x04\x08\xf6w\xc5\xd0\0\xbf\x8d\x11`\x04\x18\x01\xfe\x0f\0\x01\xc4$^a\xc0\xb7\x8b\x97\x8e\n"
" \x80\xed;\xee\x14\x06""d\xa2\x0c\x13|\xf0\x11\xdcG\n"
"pD\xed""f\xe2""5\xf2\xd2\xb9\r\xc9""e\x7f\xdd""6v\xc7^\xfbT\xbf""0\x19\xf4\xaa\x10\x02J\x0e\xd0\x0e\x90\x83\xc2\x10\xa0\r\x10\x98\xfd\xb7\x1c\xb6\x1e\xd4O\x01\xe8),7\xe3Zjy\xb7""0`\xda\xdbtK{\x95\xe7\xc7t\xac'\xc3\xee\x15\x10\xb0\xba\xe3\xd3zW\xc3\x1b"
"T\x85\xc0Zk]\xc9*\xc4V\xe1PK\x06\x02\x89\xc0&\xc2\xed\xb2(\xbb\xd5\r\x17\x98\xf2?i. \x11\xed\x02v\xf3>\xdb\x06H\x08Z\xf0l9\xe6'Ku:\x11\x1a\0\x10\x01Pb\x17\xd7\xc3\x89i\x8f]\xd9\t\r\xfa=&\xf1""2\xcb\x9d\x95v\xd0\xb8wim\xef\\\xd4\x13\xe4\xf4\x83\0@\xa3"
"_\xb3\x11`\xd8\x80\xaf""F0\xdb\x08\xf1\x03\xcb \0\0\0\0IEND\xae""B`\x82";

const char* stop_png = (const char*) temp_binary_data_24;

//================== stripe.png ==================
static const unsigned char temp_binary_data_25[] =
"\x89PNG\r\n"
"\x1a\n"
"\0\0\0\rIHDR\0\0\0.\0\0\0\\\x08\x02\0\0\0\x9e""1\x10\xaa\0\0\0\tpHYs\0\0\x0b\x13\0\0\x0b\x13\x01\0\x9a\x9c\x18\0\0\x05\xddiTXtXML:com.adobe.xmp\0\0\0\0\0<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?> <x:xmpmeta xmlns:x=\"adobe:ns:"
"meta/\" x:xmptk=\"Adobe XMP Core 5.6-c142 79.160924, 2017/07/13-01:06:39        \"> <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"> <rdf:Description rdf:about=\"\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" xmlns:dc=\"http://purl."
"org/dc/elements/1.1/\" xmlns:photoshop=\"http://ns.adobe.com/photoshop/1.0/\" xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" xmlns:stEvt=\"http://ns.adobe.com/xap/1.0/sType/ResourceEvent#\" xmp:CreatorTool=\"Adobe Photoshop CC (Windows)\" xmp:Create"
"Date=\"2018-05-12T23:57:17-04:00\" xmp:ModifyDate=\"2018-05-13T00:41:48-04:00\" xmp:MetadataDate=\"2018-05-13T00:41:48-04:00\" dc:format=\"image/png\" photoshop:ColorMode=\"3\" photoshop:ICCProfile=\"sRGB IEC61966-2.1\" xmpMM:InstanceID=\"xmp.iid:f20"
"fc3da-04ce-264a-a77c-348a16952c8a\" xmpMM:DocumentID=\"xmp.did:e32a2dd7-fc13-054c-b890-1dcaabc49f99\" xmpMM:OriginalDocumentID=\"xmp.did:e32a2dd7-fc13-054c-b890-1dcaabc49f99\"> <xmpMM:History> <rdf:Seq> <rdf:li stEvt:action=\"created\" stEvt:instance"
"ID=\"xmp.iid:e32a2dd7-fc13-054c-b890-1dcaabc49f99\" stEvt:when=\"2018-05-12T23:57:17-04:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\"/> <rdf:li stEvt:action=\"saved\" stEvt:instanceID=\"xmp.iid:f20fc3da-04ce-264a-a77c-348a16952c8a\" stEvt"
":when=\"2018-05-13T00:41:48-04:00\" stEvt:softwareAgent=\"Adobe Photoshop CC (Windows)\" stEvt:changed=\"/\"/> </rdf:Seq> </xmpMM:History> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end=\"r\"?>,\xbal\x12\0\0\x01\xc4IDATh\x81\xcd\xd9\xc9\x8d"
"\x84""0\x10@\xd1\x0f\x9a\xb8H\x81p:\x06\xc2!=80\x07Z\xc3\x80\x0c^j\xc3w\x97\xbe\x9e}\xabn\x18\x86""eY\x10\x9c\xcf\xe7\xd3u\xdd""8\x8e\x92!@\xbf\xae\xabp\x04 \xef\0\xfam\xdb$\xf7w\x12y\x07\xd0\xcbG\xa8\x90\xcc\xf3,JQ$\xd9\xb6M\xaa\xa2""E\x82\xe4\x81tI"
"\xa6i\x12\xa9(\x92\xd0\xac\xa2N\xd2\x9e\x82""6Ic\x8a\x05Ic\n"
"\x06$-)F$-)\xd8\x90T\xa7\xd8\x91T\xa7`FR\x97""bJR\x97\x82%IE\x8a""5IE\n"
"\xc6$\xa5)\x0e$\xa5)\xd8\x93\x14\xa5\xf8\x90\x14\xa5\xe0""B\x92Oq#\xc9\xa7\xe0""E\x92I\xf1$\xc9\xa4\xe0H\xf2\x94\xe2L\xf2\x94\x82/\xc9m\x8a?\xc9m\n"
"\xee$\xe9\x94\x10\x92t\n"
"\x11$\x89\x94(\x92""D\n"
"A$\xd7\x94@\x92k\n"
"q$\xa7\x94X\x12.*\x81$GJ8\t\xffUbI\xbe)o \xe1O%\x9c\x04\xe8_B\xc2\xae\xf2\x06\x12\xa0\x7f\t\t\xd0\xbf\x84\x04\xf8\x91\x8f@L\xb2\xffW\x85\xcd\x87\n"
"\xc9""8\x8e\n"
")*$\xc8\xf7""AZ$\n"
")Z$\xd2\x14""E\x12i\x8a\"\x89(E\x97""D\x94\xa2K\xd2\x9e\xa2N\xd2\x9e\xa2N\xd2\x98""bA\xd2\x98""bA\xd2\x92""bD\xd2\x92""bDR\x9d""bGR\x9d""bGR\x97""bJR\x97""bJR\x91""bMR\x91""bMR\x9a\xe2@R\x9a\xe2@R\x94\xe2""CR\x94\xe2""C\x92Oq#\xc9\xa7\xb8\x91""dR<I2)"
"\x9e$O)\xce$O)\xce$\xb7)\xfe$\xb7)\xfe$\xe9\x94\x10\x92tJ\x08I\"%\x8a$\x91\x12""ErM\t$\xb9\xa6\x04\x92\x9cRbIN)\xb1$GJ8\xc9\x91\x12N\xf2My\x03\t\xf0\x0b""D\xda\x9b\t\xb2\x19\xe1S\0\0\0\0IEND\xae""B`\x82";

const char* stripe_png = (const char*) temp_binary_data_25;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x7fffe188:  numBytes = 5536; return conditionScriptTemplate_js;
        case 0x5c8768cb:  numBytes = 646; return filterScriptTemplate_js;
        case 0xa23dd44c:  numBytes = 4737; return genericScriptTemplate_js;
        case 0x01c43842:  numBytes = 1271; return midiScriptTemplate_js;
        case 0xb21f5457:  numBytes = 1415; return moduleScriptTemplate_js;
        case 0x83ff2424:  numBytes = 798; return oscScriptTemplate_js;
        case 0xb2ba4d21:  numBytes = 1091; return streamingScriptTemplate_js;
        case 0xb02b7677:  numBytes = 36971; return about_png;
        case 0xbb8f218b:  numBytes = 3269; return add_png;
        case 0xd9f427f3:  numBytes = 3998; return connected_png;
        case 0x961b4a48:  numBytes = 3448; return default_chalayout;
        case 0x488efa25:  numBytes = 3820; return disconnected_png;
        case 0xd4093963:  numBytes = 42153; return icon_png;
        case 0xb967d40f:  numBytes = 3754; return in_png;
        case 0x2fc6dfca:  numBytes = 2624; return nextcue_png;
        case 0xbe0a39b8:  numBytes = 4041; return out_png;
        case 0x107086a8:  numBytes = 584; return padlock_png;
        case 0x31751c60:  numBytes = 701; return pause_png;
        case 0x7000b29e:  numBytes = 1849; return play_png;
        case 0xee61ea98:  numBytes = 952; return playing_png;
        case 0xab17ebac:  numBytes = 16163; return playpause_png;
        case 0x8ff9208a:  numBytes = 2536; return prevcue_png;
        case 0x9da18b7e:  numBytes = 1959; return smallstripe_png;
        case 0x10f1dc34:  numBytes = 3649; return snap_png;
        case 0x663b186c:  numBytes = 1978; return stop_png;
        case 0xa58f5337:  numBytes = 2043; return stripe_png;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "conditionScriptTemplate_js",
    "filterScriptTemplate_js",
    "genericScriptTemplate_js",
    "midiScriptTemplate_js",
    "moduleScriptTemplate_js",
    "oscScriptTemplate_js",
    "streamingScriptTemplate_js",
    "about_png",
    "add_png",
    "connected_png",
    "default_chalayout",
    "disconnected_png",
    "icon_png",
    "in_png",
    "nextcue_png",
    "out_png",
    "padlock_png",
    "pause_png",
    "play_png",
    "playing_png",
    "playpause_png",
    "prevcue_png",
    "smallstripe_png",
    "snap_png",
    "stop_png",
    "stripe_png"
};

const char* originalFilenames[] =
{
    "conditionScriptTemplate.js",
    "filterScriptTemplate.js",
    "genericScriptTemplate.js",
    "midiScriptTemplate.js",
    "moduleScriptTemplate.js",
    "oscScriptTemplate.js",
    "streamingScriptTemplate.js",
    "about.png",
    "add.png",
    "connected.png",
    "default.chalayout",
    "disconnected.png",
    "icon.png",
    "in.png",
    "nextcue.png",
    "out.png",
    "padlock.png",
    "pause.png",
    "play.png",
    "playing.png",
    "playpause.png",
    "prevcue.png",
    "smallstripe.png",
    "snap.png",
    "stop.png",
    "stripe.png"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
