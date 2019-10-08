[@react.component]
let make = (~text) => {
  <header className="App-header">
    <h2>{ReasonReact.string(text)}</h2>
  </header>;
};
