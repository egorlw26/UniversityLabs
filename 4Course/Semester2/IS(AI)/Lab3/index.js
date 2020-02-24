var canvas = document.createElement('canvas')
document.body.appendChild(canvas)

canvas.width = window.innerWidth*0.95;
canvas.height = window.innerHeight*0.95;
canvas.style = "position: center;"
var offsetForMaze = 1;

var context = canvas.getContext('2d')

/// set parameters
var rows = 15;
var columns = 15;
var w = canvas.width / rows-1;
var h = canvas.height / columns-1;

//simple way to draw line
function draw_line(startX, startY, endX, endY){
  context.beginPath();
  context.moveTo(startX, startY);
  context.lineTo(endX, endY);
  context.stroke();
}

function randomInt(a, b) {
  return Math.floor(a + Math.random() * (b - a));
}

function randomChoice(list) {
  const id = randomInt(0, list.length);
  return list[id];
}

const Dirs = [
  [-1, 0],
  [0, 1],
  [1, 0],
  [0, -1]
];

class Cell {
  constructor(type = 0) {
    this.type = type;
    this.walls = [true, true, true, true];
  }

  draw(x, y) {
    context.strokeStyle = 'black'
    for (let i = 0; i < 4; ++i) {
      const next_x = x + Dirs[(i + 1) % 4][0];
      const next_y = y + Dirs[(i + 1) % 4][1];
      if (this.walls[i]) {
        draw_line(offsetForMaze + x * w, offsetForMaze + y * h,
          offsetForMaze + next_x * w, offsetForMaze + next_y * h);
      }
      x = next_x;
      y = next_y;
    }
  }
}

class Maze {
  constructor(width, height) {
    this.width = width
    this.height = height
    this.initGrid()
  }

  initGrid() {
    this.maze = [];
    for (let y = 0; y < this.height; ++y) {
      var row = [];
      for (let x = 0; x < this.width; ++x)
        row.push(new Cell(0));
      this.maze.push(row);
    }
  }

  isInGrid(x, y) {
    return (x >= 0 && y >= 0 && x < this.width && y < this.height);
  }

  dfsGenerator() {
    var visited = [];
    for (let y = 0; y < this.height; ++y) {
      var row = [];
      for (let x = 0; x < this.width; ++x)
        row.push(false);
      visited.push(row);
    }
    var stack = [
      [randomInt(0, this.width), randomInt(0, this.height)]
    ];
    while (stack.length > 0) {
      const curr_pos = stack[stack.length - 1];
      const x = curr_pos[0];
      const y = curr_pos[1];
      visited[y][x] = true;
      var available_dirs = [];
      for (let i = 0; i < 4; ++i) {
        const cell_x = x + Dirs[i][0];
        const cell_y = y + Dirs[i][1];
        if (this.isInGrid(cell_x, cell_y) && !visited[cell_y][cell_x])
          available_dirs.push(i);
      }
      if (available_dirs.length == 0) {
        stack.pop();
        continue;
      }
      const dir_id = randomChoice(available_dirs);
      const next_x = x + Dirs[dir_id][0];
      const next_y = y + Dirs[dir_id][1];
      stack.push([next_x, next_y]);
      this.maze[y][x].walls[dir_id] = false;
      this.maze[next_y][next_x].walls[(dir_id + 2) % 4] = false;
    }
  }

  draw() {
    context.rect(0, 0, this.width * 20, this.height * 20);
    for (let y = 0; y < this.height; ++y)
      for (let x = 0; x < this.width; ++x)
        this.maze[y][x].draw(x, y);
  }
}

class Player{
  constructor(maze){
    this.path = [];
    this.found = false;
    this.visited = [];
    this.maze = maze;
  }
  dfsPathfinderStart(){
    this.found = false;
    this.visited = [];
    for(var i = 0; i<rows; i++){
      var row = [];
      for(var j = 0; j<columns; j++)
        row.push(false);
      this.visited.push(row);
    }

    this.path =[
      [rows-1, columns-1]
    ];
  }

  dfsPathfinderMakeStep(targetX, targetY) {
      if(this.path.length > 0 && !this.found) {
      const curr_pos = this.path[this.path.length -1];
      const x = curr_pos[0];
      const y = curr_pos[1];
      this.visited[y][x] = true;

      /// We found our target!
      if(x === targetX && y === targetY){
        this.found = true;
        return;
      }

      var available_dirs = []
      for(var i = 0; i<4; i++){
        const cell_x = x + Dirs[i][0];
        const cell_y = y + Dirs[i][1];
        if(this.maze.isInGrid(cell_x, cell_y) && !this.visited[cell_y][cell_x] && !this.maze.maze[y][x].walls[i])
          available_dirs.push(i);
      }

      if (available_dirs.length == 0) {
        this.path.pop();
        return;
      }

      const dir_id = available_dirs[0];
      const next_x = x + Dirs[dir_id][0];
      const next_y = y + Dirs[dir_id][1];
      this.path.push([next_x, next_y]);
    }
  }
}

function drawPlayerPath(path){
  context.strokeStyle = 'red';
  for(var i = 1; i<path.length; i++)
  draw_line(path[i-1][0]*w + w/2, path[i-1][1]*h + h/2, 
    path[i][0]*w + w/2, path[i][1]*h + h/2);
}

function drawEverything(maze, player){
  maze.draw();
  drawPlayerPath(player.path);
}

var maze = new Maze(rows, columns);
maze.dfsGenerator();

var player = new Player(maze);
player.dfsPathfinderStart();


function update(){
  context.clearRect(0, 0, canvas.width, canvas.height);
  player.dfsPathfinderMakeStep(0, 0);
  drawEverything(maze, player);
  setTimeout(update, 40);
}

update();