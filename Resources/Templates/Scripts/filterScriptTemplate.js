/* ********** FILTER SPECIFIC SCRIPTING **********************

	The "local" variable refers to the object containing the scripts. In this case, the local variable refers to the filter.
	It means that you can access any control inside  this filter by accessing it through its address.
*/

/*
 This function will be called each time the filter is processed, and expects a return value.
 This function only exists because the script is in a filter
 The "inputs" argument is an array of all the parameters that are being filtered. Each element can be either a single value or an array of values itself (if it's a Color or Point 2D/3D for instance)

 The minValues and max Values are arrays of the same size as inputs, containing the value range of the input if applicable 

 If this filter is inside a multiplexed mapping, multiplexIndex is the current index of the multiplex list

 The result must be an array of the same size as the inputValues

*/

var multiplier = script.addFloatParameter("Multiplier", "The multiplication factor to apply to all input values", 2, 0, 10);

function filter(inputs, minValues, maxValues, multiplexIndex)
{
	var result = [];
	for(var i = 0; i < inputs.length; i++)
	{
		result[i] = inputs[i] * multiplier.get(); //Basic multiplication of all the inputs by the script parameter myFloatParam
	}

	return result;
}