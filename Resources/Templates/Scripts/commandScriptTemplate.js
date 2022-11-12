/* ********** COMMAND SPECIFIC SCRIPTING ********************** */


/*
If this script is in a mapping, then this will be called whenever the value from the mapping is updated
script parameters linked values are added to the function signature, so you can modify this function depending on the parameters you manually added to the script
*/
function setValue(values, multiplexIndex) {
    script.log("Set values :");
    for (var i = 0; i < values.length; i++) {
        script.log(" > " + values[i]);
    }
}

/*
This will be called either when the consequence is triggered if it is in an Action, or just after setValue() if it is in a Mapping
script parameters linked values are added to the function signature, so you can modify this function depending on the parameters you manually added to the script
*/
function trigger(multiplexIndex) {
    script.log("Trigger !");
}