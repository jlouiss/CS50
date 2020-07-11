/* People starred in a movie where Kevin Bacon starred */
SELECT name FROM people
WHERE people.id IN (
    SELECT DISTINCT person_id FROM people
    JOIN stars ON people.id = stars.person_id
    JOIN movies ON movies.id = stars.movie_id
    WHERE movies.id IN (
        SELECT movies.id FROM movies
        JOIN stars ON movies.id = stars.movie_id
        JOIN people ON people.id = stars.person_id
        WHERE people.name = "Kevin Bacon"
        AND birth = 1958
    )
)
AND people.name != "Kevin Bacon";
