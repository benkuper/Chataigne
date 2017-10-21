data.stateManager.items.forEach(function(state)
{
	state.actions.items.forEach(function(action)
    {
    	action.type = "Action";
    	console.log(action);
    });
    
    state.mappings.items.forEach(function(mapping)
    {
    	mapping.type = "Mapping";
        console.log(mapping);
    });
});