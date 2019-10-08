Utils.requireCSS("./App.css");

type state = {
  isLoading: bool,
  results: list(MovieData.movie),
  errorMessage: string,
};

type action =
  | Search
  | SearchSuccess(list(MovieData.movie))
  | SearchFailure(string);

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Search => {...state, isLoading: true}
        | SearchFailure(errorMessage) => {
            ...state,
            isLoading: false,
            errorMessage,
          }
        | SearchSuccess(newResults) => {
            ...state,
            isLoading: false,
            results: newResults,
          }
        },
      {errorMessage: "", isLoading: false, results: []},
    );

  let searchFunc = text => {
    dispatch(Search);
    MovieData.Api.searchMovies(text)
    |> Js.Promise.then_(results => {
         dispatch(SearchSuccess(results));
         Js.Promise.resolve();
       })
    |> Js.Promise.catch(message => SearchFailure(message) |> dispatch)
    |> ignore;
  };

  <div className="App">
    <Header text="HOOKED" />
    <Search searchFunc />
    <p className="App-intro">
      {React.string("Sharing a few of our favourite movies")}
    </p>
    <div className="movies">
      {state.isLoading
         ? ReasonReact.string("Loading...")
         : state.results
           |> Array.of_list
           |> Array.map((movie: MovieData.movie) =>
                <Movie key={movie.id} movie />
              )
           |> ReasonReact.array}
    </div>
  </div>;
};