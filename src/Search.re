[@react.component]
let make = (~searchFunc: string => unit) => {
  let (input, setInput) = React.useState(_ => "");

  let handleSearchInputChanges = event => {
    let value = ReactEvent.Form.target(event)##value;
    setInput(value);
  };

  let callSearchFunction = event => {
    ReactEvent.Form.preventDefault(event);
    searchFunc(input);
  };

  <form className="search" onSubmit=callSearchFunction>
    <input value=input onChange=handleSearchInputChanges type_="text" />
    <button type_="submit">
      {React.string("SEARCH")}
    </button>
  </form>;
};