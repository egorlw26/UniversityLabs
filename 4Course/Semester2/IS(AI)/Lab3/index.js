var canvas = document.createElement('canvas')
document.body.appendChild(canvas)

canvas.width = 600;
canvas.height = 600;

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

function image_test()
{
  var img = new Image();
  img.src = 'smile.jpg';
  console.log(img);
  document.body.appendChild(img);
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
    for (let i = 0; i < 4; ++i) {
      const next_x = x + Dirs[(i + 1) % 4][0];
      const next_y = y + Dirs[(i + 1) % 4][1];
      if (this.walls[i]) {
        draw_line(x * w, y * h, next_x * w, next_y * h);
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
    this.maze = maze;
}

  dfsPathfinder(targetX, targetY){
    var visited = [];
    for(var i = 0; i<rows; i++){
      var row = [];
      for(var j = 0; j<columns; j++)
        row.push(false);
      visited.push(row);
    }

    var stack=[
      [Math.floor(rows/2) - 1, Math.floor(columns/2) - 1]
    ];

    while(stack.length > 0) {
      const curr_pos = stack[stack.length -1];
      const x = curr_pos[0];
      const y = curr_pos[1];
      visited[y][x] = true;

      console.log(x, y, targetX, targetY);
      /// We found our target!
      if(x === targetX && y === targetY){
        this.path = stack;
        return;
      }

      var available_dirs = []
      for(var i = 0; i<4; i++){
        const cell_x = x + Dirs[i][0];
        const cell_y = y + Dirs[i][1];
        if(this.maze.isInGrid(cell_x, cell_y) && !visited[cell_y][cell_x] && !this.maze.maze[y][x].walls[i])
          available_dirs.push(i);
      }

      if (available_dirs.length == 0) {
        stack.pop();
        continue;
      }

      const dir_id = available_dirs[0];
      const next_x = x + Dirs[dir_id][0];
      const next_y = y + Dirs[dir_id][1];
      stack.push([next_x, next_y]);
    }
  }
}

var maze = new Maze(rows, columns);
maze.dfsGenerator();
maze.draw()

var player = new Player(maze);
player.dfsPathfinder(0, 0);

path = player.path;
console.log(path);
context.strokeStyle = 'red';
for(var i = 1; i<path.length; i++)
  draw_line(path[i-1][0]*w + w/2, path[i-1][1]*h + h/2, path[i][0]*w + w/2, path[i][1]*h + h/2);


//image_test();