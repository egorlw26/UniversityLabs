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
        line(x * 20, y * 20, next_x * 20, next_y * 20);
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
    rect(0, 0, this.width * 20, this.height * 20);
    for (let y = 0; y < this.height; ++y)
      for (let x = 0; x < this.width; ++x)
        this.maze[y][x].draw(x, y);
  }
}

var maze = null;

function setup() {
  createCanvas(1000, 1000);
  maze = new Maze(50, 50);
  maze.dfsGenerator();
  console.log(maze.maze[1][1].walls);
}

function draw() {
  background(220);
  maze.draw();
}