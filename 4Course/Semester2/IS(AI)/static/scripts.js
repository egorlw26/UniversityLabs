
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

function refresh(){
	alert("Hi!");
	var input = document.getElementById("mainInput");
	input.value = "asdasdas";
}
