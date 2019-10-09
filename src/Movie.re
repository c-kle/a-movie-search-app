let defaultPlaceholderPoster = "https://m.media-amazon.com/images/G/01/imdb/images/nopicture/medium/film-3385785534._CB483791896_.png";

let defaultPosterIfNA = posterUrl =>
  posterUrl === "N/A" ? defaultPlaceholderPoster : posterUrl;

[@react.component]
let make = (~movie: MovieData.movie) => {
  <div className="movie">
    <h2> {ReasonReact.string(movie.title)} </h2>
    <div>
      <img
        width="200"
        alt={"The movie titled " ++ movie.title}
        src={
          Belt.Option.getWithDefault(movie.poster, defaultPlaceholderPoster)
          |> defaultPosterIfNA
        }
      />
    </div>
    <p> {React.string(movie.year)} </p>
  </div>;
};