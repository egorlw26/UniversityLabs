function delete_row(db_name, table_name, index){
	$.post("/databases/"+db_name+"/tables/"+table_name +"/delete_row/"+index, refresh);
}

function delete_table(db_name, table_name){
	$.post("/databases/"+db_name+"/tables/"+table_name+"/delete", refresh);
}
	
function refresh(){
	document.location.reload(true);
}

function add_row(db_name, table_name) {
	let vals = [];
	$('#'+table_name+' input').each(function(index){vals.push($(this).val())});
	$.post("/databases/"+db_name+"/tables/"+table_name+"/add_row", {vals: vals}, refresh);
}

function add_column(db_name){
	let table_name = $("#hidden_table_name").val();
	let column_name = $("#column_name_input").val();
	console.log(table_name);
	console.log(column_name);
	$.post("/databases/"+db_name+"/tables/"+table_name+"/add_column", {name: column_name}, refresh);
}

function add_table(db_name){
	let name = $("#table_name_input").val();
	console.log(name);
	$.post("/databases/"+db_name+"/tables/add_table", {name: name}, refresh);
}

function remove_column(db_name, table_name, column_name){
	$.post("/databases/"+db_name+"/tables/"+table_name+"/remove_column/"+column_name, refresh);
}

function add_union_table(db_name){
	let f_name = $("#union_ft_name_input").val();
	let s_name = $("#union_st_name_input").val();
	let c_name = $("#union_c_name_input").val();
	let n_name = $("#union_nt_name_input").val();
	$.post("/databases/"+db_name+"/tables/union_table", {f_name: f_name, s_name: s_name, c_name: c_name, n_name: n_name}, refresh);
}


function delete_table_mongo(database_name, table_name){
	$.post("/mongo_databases/"+database_name+"/tables/"+table_name+"/delete", refresh);
}

function remove_column_mongo(database_name, table_name, column_name){
	$.post("/mongo_databases/"+database_name+"/tables/"+table_name+"/delete_column/"+column_name, refresh);
}

function delete_row_mongo(database_name, table_name, row_index){
	$.post("/mongo_databases/"+database_name+"/tables/"+table_name+"/delete_row/"+row_index, refresh);
}

function add_row_mongo(db_name, table_name){
	let vals = [];
	$('#'+table_name+' input').each(function(index){vals.push($(this).val())});
	$.post("/mongo_databases/"+db_name+"/tables/"+table_name+"/add_row", {vals: vals}, refresh);
}

function add_table_mongo(db_name){
	let name = $("#table_name_input").val();
	$.post("/mongo_databases/"+db_name+"/tables/add_table", {name: name}, refresh);
}

function add_column_mongo(db_name){
	let table_name = $("#hidden_table_name").val();
	let column_name = $("#column_name_input").val();
	console.log(table_name);
	console.log(column_name);
	$.post("/mongo_databases/"+db_name+"/tables/"+table_name+"/add_column", {name: column_name}, refresh);
}

function add_union_table_mongo(db_name){
	let f_name = $("#union_ft_name_input").val();
	let s_name = $("#union_st_name_input").val();
	let c_name = $("#union_c_name_input").val();
	let n_name = $("#union_nt_name_input").val();
	$.post("/mongo_databases/"+db_name+"/tables/union_table", {f_name: f_name, s_name: s_name, c_name: c_name, n_name: n_name}, refresh);
}

$(document).ready(function(){
    pop_up_t_hide();
	pop_up_c_hide();
	pop_up_u_hide();
});
function pop_up_t_show(){
	$("#table_name_input").val('enter');
    $("#popup_table").show();
}
function pop_up_t_hide(){
	$("#table_name_input").val('');
    $("#popup_table").hide();
}

function pop_up_c_show(table_name){
	$("#hidden_table_name").val(table_name);
	$("#column_name_input").val('enter');
    $("#popup_column").show();
}
function pop_up_c_hide(){
	$("#column_name_input").val('');
    $("#popup_column").hide();
}

function pop_up_u_show(){
	$("#union_ft_name_input").val('enter');
	$("#union_st_name_input").val('enter');
	$("#union_c_name_input").val('enter');
    $("#popup_union").show();
}
function pop_up_u_hide(){
	$("#union_ft_name_input").val('');
	$("#union_st_name_input").val('');
	$("#union_c_name_input").val('');
    $("#popup_union").hide();
}