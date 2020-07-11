/* All harry potter movies order by year */
SELECT title, year FROM movies
WHERE title LIKE "Harry Potter%"
ORDER BY year;