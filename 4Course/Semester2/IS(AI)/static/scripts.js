
function onEnter(){
	var input = document.getElementById("mainInput");
	var textArea = document.getElementById("mainTextArea");
	input.addEventListener("keyup", function(event) {
		if (event.keyCode === 13 && !(input.value.trim() === "")) {
			event.preventDefault();
			textArea.value+="\n You:\t"+input.value+'\n';
			$.post("/test", {msg: input.value}, function(response){
				textArea.value+="\n Bot:\t"+response.response+'\n';
			});
			input.value="";
		}
	});
}

function fillCategorySelect(){
	
}

function addCategory(){
	var input = document.getElementById("categoryInput");
	var name = input.value;
	input.value = "";
	$.post("/addCategory/"+name);
}

function refresh(){
}
