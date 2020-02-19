open Imandra_prelude;

type player =
  | X
  | O;

type square_state = option(player);

type grid = {
  a: square_state,
  b: square_state,
  c: square_state,
  d: square_state,
  e: square_state,
  f: square_state,
  g: square_state,
  h: square_state,
  i: square_state,
};

let a_grid = {
  a: Some(O),
  b: Some(X),
  c: Some(O),
  d: None,
  e: None,
  f: Some(X),
  g: None,
  h: None,
  i: None,
};

/* [@program] */
/* let doc_of_square_state = (s: square_state) => */
/*   switch (s) { */
/*   | Some(X) => "X" */
/*   | Some(O) => "O" */
/*   | None => " " */
/*   }; */
/* [@program] */
/* let doc_of_grid = (g: grid) => { */
/*   let f = doc_of_square_state; */
/*   Document.s( */
/*     "\n" */
/*     ++ f(g.a) */
/*     ++ "|" */
/*     ++ f(g.b) */
/*     ++ "|" */
/*     ++ f(g.c) */
/*     ++ "\n" */
/*     ++ "-----" */
/*     ++ "\n" */
/*     ++ f(g.d) */
/*     ++ "|" */
/*     ++ f(g.e) */
/*     ++ "|" */
/*     ++ f(g.f) */
/*     ++ "\n" */
/*     ++ "-----" */
/*     ++ "\n" */
/*     ++ f(g.g) */
/*     ++ "|" */
/*     ++ f(g.h) */
/*     ++ "|" */
/*     ++ f(g.i), */
/*   ); */
/* }; */
/* #install_doc doc_of_grid; */
type move =
  | A
  | B
  | C
  | D
  | E
  | F
  | G
  | H
  | I;

type game_state = {
  grid,
  last_player: option(player),
};

type game_status =
  | Won(player)
  | InProgress
  | InvalidMove(move)
  | Tied;

let initial_game = {
  grid: {
    a: None,
    b: None,
    c: None,
    d: None,
    e: None,
    f: None,
    g: None,
    h: None,
    i: None,
  },
  last_player: None,
};

let initial_player = X;

let value = ({grid: {a, b, c, d, e, f, g, h, i}, _}) =>
  fun
  | A => a
  | B => b
  | C => c
  | D => d
  | E => e
  | F => f
  | G => g
  | H => h
  | I => i;

let is_winning = ({grid: {a, b, c, d, e, f, g, h, i}, _}, player) => {
  let winning_state = (Some(player), Some(player), Some(player));
  (a, b, c) == winning_state
  || (d, e, f) == winning_state
  || (g, h, i) == winning_state
  || (a, d, g) == winning_state
  || (b, e, h) == winning_state
  || (c, f, i) == winning_state
  || (a, e, i) == winning_state
  || (c, e, g) == winning_state;
};

/* instance((game) => is_winning(game, X)); */
let other_player =
  fun
  | X => O
  | O => X;

let move_counts = ({a, b, c, d, e, f, g, h, i}) =>
  List.fold_right(
    (el, (x, o)) =>
      switch (el) {
      | None => (x, o)
      | Some(X) => (x + 1, o)
      | Some(O) => (x, o + 1)
      },
    ~base=(0, 0),
    [a, b, c, d, e, f, g, h, i],
  );

let is_valid_grid = (grid, last_player) => {
  let (x, o) = move_counts(grid);

  x
  + o == 0
  && last_player == None
  || x
  - o == 1
  && last_player == Some(X)
  || x == o
  && last_player == Some(O);
};

let is_tie = ({grid: {a, b, c, d, e, f, g, h, i}, _}) =>
  List.for_all((!=)(None), [a, b, c, d, e, f, g, h, i]);

/* instance((game) => is_valid_grid(game.grid, game.last_player)); */
/* instance((game) => {
   let (x, o) = move_counts(game.grid);
   is_valid_grid(game.grid, game.last_player) && ((x - o) >= 2)
   }); */
/* verify((game) => {
   let (x, o) = move_counts(game.grid);
   not(is_valid_grid(game.grid, game.last_player) && ((x - o) >= 2))
   }); */
let is_valid_game = game => {
  let winning_X = is_winning(game, X);
  let winning_O = is_winning(game, O);
  is_valid_grid(game.grid, game.last_player)
  && (!winning_X && !winning_O || winning_X != winning_O);
};

/* instance((game) => is_valid_game(game)); */
/* instance((game) => is_valid_game(game) && is_winning(game, X)); */
/* instance((game) => is_valid_game(game) && is_winning(game, O)); */
/* instance((game) => is_valid_game(game) && is_tie(game)); */
let is_valid_move = (game, player, move) =>
  !(is_winning(game, X) || is_winning(game, O) || is_tie(game))
  && is_valid_game(game)
  && (
    game.last_player == None
    && player == initial_player
    || game.last_player == Some(other_player(player))
  )
  && value(game, move) == None;

let play_move = ({grid, _}, player, move) => {
  let play = Some(player);
  let grid =
    switch (move) {
    | A => {...grid, a: play}
    | B => {...grid, b: play}
    | C => {...grid, c: play}
    | D => {...grid, d: play}
    | E => {...grid, e: play}
    | F => {...grid, f: play}
    | G => {...grid, g: play}
    | H => {...grid, h: play}
    | I => {...grid, i: play}
    };
  {grid, last_player: play};
};

/* play_move(initial_game, X, A); */
let status = game =>
  if (is_winning(game, X)) {
    Won(X);
  } else if (is_winning(game, O)) {
    Won(O);
  } else if (is_tie(game)) {
    Tied;
  } else {
    InProgress;
  };

let play = ({last_player, _} as game, move) => {
  let player =
    switch (last_player) {
    | None => initial_player
    | Some(player) => other_player(player)
    };
  if (is_valid_move(game, player, move)) {
    let game = play_move(game, player, move);
    (game, status(game));
  } else {
    (game, InvalidMove(move));
  };
};
/* verify((game, player, move) => */
/*   (is_valid_game(game) && is_valid_move(game, player, move)) */
/*   ==> is_valid_game(play_move(game, player, move)) */
/* ); */
/* is_valid_game(CX.game); */
/* is_valid_move(CX.game, CX.player, CX.move); */
/* is_valid_game(play_move(CX.game, CX.player, CX.move)); */
