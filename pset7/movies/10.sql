/* names of people who directed a movie with rating >= 9.0 */
SELECT name FROM people
WHERE people.id IN (
    SELECT DISTINCT person_id FROM people
    JOIN directors ON people.id = directors.person_id
    JOIN movies ON movies.id = directors.movie_id
    JOIN ratings ON ratings.movie_id = movies.id
    WHERE rating >= 9
);