/* ********** PJLINK  MODULE SPECIFIC SCRIPTING ********************* */


// pjLinkDataReceived callback allows you to receive projector's data
// id is the number of the projector in the module's projectors list
// message is the data received

function pjLinkDataReceived(id, message)
{
    script.log("PJLink data received from projector #" + id + " : " + message);
}