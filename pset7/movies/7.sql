/**
 * movies release in 2010 with ratings
 * descending order by rating
 */
SELECT title, rating FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE year = 2010
ORDER BY rating DESC, title;