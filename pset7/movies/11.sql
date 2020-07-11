/**
 * title of 5 highest rated movies
 * where Chadwick Boseman starred in
 * starting with highest rated
 */
SELECT title FROM movies
JOIN ratings ON movies.id = ratings.movie_id
JOIN stars ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE name = "Chadwick Boseman"
ORDER BY rating DESC LIMIT 5;