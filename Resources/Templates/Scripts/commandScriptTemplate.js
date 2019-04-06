/* ********** COMMAND SPECIFIC SCRIPTING ********************** */


/*
If this script is in a mapping, then this will be called whenever the value from the mapping is updated
*/
function setValue(value)
{
	script.log("Set value "+value);
}

/*
This will be called either when the consequence is triggered if it is in an Action, or just after setValue() if it is in a Mapping
*/
function trigger()
{
	script.log("Trigger !");
}