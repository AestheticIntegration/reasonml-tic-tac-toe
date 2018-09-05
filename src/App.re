[@bs.module "emotion"] external css : string => string = "css";

type status =
  | InProgress
  | Finished;

type state = {status};

type action =
  | Finish
  | Restart;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {status: InProgress},
  reducer: (action, _state) =>
    switch (action) {
    | Finish => ReasonReact.Update({status: Finished})
    | Restart => ReasonReact.Update({status: InProgress})
    },
  render: self =>
    <div
      className=(
        css("display: flex; flex-direction: column; align-items: center;")
      )>
      <h1> (ReasonReact.string("Tic Tac Toe")) </h1>
      <TicTacToe onGameFinished=(() => self.send(Finish)) />
    </div>,
};
