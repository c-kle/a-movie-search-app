let defaultPlaceholderPoster = "https://m.media-amazon.com/images/M/MV5BMTczNTI2ODUwOF5BMl5BanBnXkFtZTcwMTU0NTIzMw@@._V1_SX300.jpg";
let apiUrl = "http://www.omdbapi.com/?apikey=32cd990";

let buildSearchUrl = text => apiUrl ++ "&s=" ++ text;

type movie = {
  id: string,
  title: string,
  poster: string,
  year: string,
};

type searchMovieResponse = {
  response: string,
  errorMessage: option(string),
  results: option(list(option(movie))),
};

let unwrapMovies = results =>
  switch (results) {
  | Some(movies) =>
    movies
    |> List.filter(optionalItem =>
         switch (optionalItem) {
         | Some(_) => true
         | None => false
         }
       )
    |> List.map(item =>
         switch (item) {
         | Some(item) => item
         }
       )
  | None => []
  };

let unwrapError = error =>
  switch (error) {
  | Some(message) => message
  | None => "Unknown error"
  };

module Api = {
  open Json.Decode;

  let decodeResults = json => {
    response: field("Response", string, json),
    errorMessage: optional(field("Error", string), json),
    results:
      optional(
        field(
          "Search",
          list(
            optional(json =>
              {
                id: field("imdbID", string, json),
                title: field("Title", string, json),
                poster: field("Poster", string, json),
                year: field("Year", string, json),
              }
            ),
          ),
        ),
        json,
      ),
  };

  let searchMovies = (text: string) =>
    Js.Promise.(
      text
      |> buildSearchUrl
      |> Fetch.fetch
      |> then_(Fetch.Response.json)
      |> then_(json => decodeResults(json) |> resolve)
      |> then_(({response, results, errorMessage}: searchMovieResponse) =>
           if (response == "True") {
             results |> unwrapMovies |> resolve;
           } else {
             errorMessage |> unwrapError |> Js.Exn.raiseError;
           }
         )
    );
};