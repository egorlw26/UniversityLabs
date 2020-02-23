
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

function trainBot(){
	$.post("/update", refresh);
}

function addCategory(){
	var input = document.getElementById("categoryInput");
	var name = input.value;
	input.value = "";
	$.post("/addCategory/"+name, refresh);
}

function addDialog(){
	var category = document.getElementById("categorySelect").value;
	var inputPhraseInput = document.getElementById("inputInput");
	var outputPhraseInput = document.getElementById("outputInput");
	var iPhrase = inputPhraseInput.value;
	var oPhrase = outputPhraseInput.value;
	inputPhraseInput.value = "";
	outputPhraseInput.value = "";
	$.post("/addDialog/"+category+'/'+iPhrase+'/'+oPhrase, refresh);
}

function refresh(){
	document.location.reload(true);
}
