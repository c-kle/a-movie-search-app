[@react.component]
let make = (~movie: MovieData.movie) => {
  <div className="movie">
      <h2>{ReasonReact.string(movie.title)}</h2>
      <div>
        <img
          width="200"
          alt={"The movie titled " ++ movie.title}
          src={movie.poster}
        />
      </div>
      <p>({React.string(movie.year)})</p>
    </div>
};