/*
	Sean McGrath, ssmcgrat@ucsc.edu
	CMPS180 Lab Assignment 2
	2/1/2016
	script4.sql
*/

-- --------------------------------------------------------------------
-- Query 1:
-- Get first and last names and phone numbers of all people who are 
-- customers of both Smith Video and Smith Books.
-- --------------------------------------------------------------------

SELECT dv_customer.first_name, dv_customer.last_name, phone
   FROM dv_customer, cb_customers, dv_address
   WHERE (dv_customer.address_id = dv_address.address_id) 
      AND (
         (dv_customer.first_name = cb_customers.first_name)
         AND (dv_customer.last_name = cb_customers.last_name)
      )
;

-- --------------------------------------------------------------------
-- Query 2:
-- How many first edition books are present in Smith Books per
-- publisher? Return publisher names and the number of first edition
-- books per publisher, sorted in descending order. 
-- --------------------------------------------------------------------

SELECT DISTINCT publisher, COUNT(*)
   FROM cb_books
   WHERE edition = 1
   GROUP BY publisher
   ORDER BY COUNT(*) DESC;

-- --------------------------------------------------------------------
-- Query 3:
-- What are the first and last names of all customers who live in the 
-- district having the most customers?
-- --------------------------------------------------------------------

SELECT first_name, last_name
   FROM mg_customers
   WHERE address_id IN
      (SELECT address_id
         FROM dv_address
         WHERE district =
           (SELECT district
            FROM dv_address
            GROUP BY district
            ORDER BY COUNT(district) DESC
            LIMIT 1
            )
      );
	  

-- --------------------------------------------------------------------
-- Query 4:
-- What rating is the most common among films in the Smith Video 
-- database, and how many films have that rating?
-- --------------------------------------------------------------------

SELECT rating, COUNT(*)
   FROM dv_film
   GROUP BY rating
   ORDER BY COUNT(*) DESC
   LIMIT 1;

-- --------------------------------------------------------------------
-- Query 5:
-- What are the first and last names of the top 10 authors when ranked
-- by the number of books each has written? (Return both author name 
-- and the number of books of top 10 authors, sorted in descending
-- order). 
-- --------------------------------------------------------------------

SELECT cb_authors.first_name, cb_authors.last_name, COUNT(*)
   FROM cb_books, cb_authors
   WHERE cb_authors.author_id = cb_books.author_id
   GROUP BY cb_authors.first_name, cb_authors.last_name
   ORDER BY COUNT(*) DESC
   LIMIT 10;




