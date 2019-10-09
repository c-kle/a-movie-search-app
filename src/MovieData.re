let apiUrl = "http://www.omdbapi.com/?apikey=32cd990";

let buildSearchUrl = text => apiUrl ++ "&s=" ++ text;

type movie = {
  id: string,
  title: string,
  poster: option(string),
  year: string,
};

type searchMovieResponse = (string, option(list(movie)), option(string));

module Api = {
  let decodeResults = json =>
    Json.Decode.(
      field("Response", string, json),
      optional(
        field(
          "Search",
          list(json =>
            {
              id: field("imdbID", string, json),
              title: field("Title", string, json),
              poster: optional(field("Poster", string), json),
              year: field("Year", string, json),
            }
          ),
        ),
        json,
      ),
      optional(field("Error", string), json),
    );

  let searchMovies = (text: string) =>
    Js.Promise.(
      text
      |> buildSearchUrl
      |> Fetch.fetch
      |> then_(Fetch.Response.json)
      |> then_(json => decodeResults(json) |> resolve)
      |> then_((response: searchMovieResponse) =>
           (
             switch (response) {
             | ("True", Some(movies), None) => (Some(movies), None)
             | (_, _, Some(errorMessage))
                 when String.length(errorMessage) > 0 => (
                 None,
                 Some(errorMessage),
               )
             | _ => (None, Some("Unknown error"))
             }
           )
           |> resolve
         )
      |> catch(error => {
           Js.log(error);
           resolve((None, Some("Errooor...")));
         })
    );
};