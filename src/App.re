Utils.requireCSS("./App.css");

type state = {
  isLoading: bool,
  result: (option(list(MovieData.movie)), option(string)),
};

type action =
  | Search
  | SearchDone((option(list(MovieData.movie)), option(string)));

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Search => {...state, isLoading: true}
        | SearchDone(newResult) => {
            ...state,
            isLoading: false,
            result: newResult,
          }
        },
      {isLoading: false, result: (Some([]), None)},
    );

  let searchFunc = text => {
    Search |> dispatch;
    MovieData.Api.searchMovies(text)
    |> Js.Promise.then_(result => {
         SearchDone(result) |> dispatch;
         Js.Promise.resolve();
       })
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
         : (
           switch (state.result) {
           | (Some(movies), None) =>
             movies
             |> Array.of_list
             |> Array.mapi((index: int, movie: MovieData.movie) =>
                  <Movie key={movie.id ++ string_of_int(index)} movie />
                )
             |> ReasonReact.array
           | (None, Some(errorMessage)) =>
             <div className="errorMessage">
               {ReasonReact.string(errorMessage)}
             </div>
           }
         )}
    </div>
  </div>;
};