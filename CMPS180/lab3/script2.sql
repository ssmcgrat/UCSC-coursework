/*
	Sean McGrath, ssmcgrat@ucsc.edu
	CMPS180 Lab Assignment 3
	16 February 2016
*/

-- Adding foreign key constraints

alter table mg_customers
add foreign key (address_id)
references dv_address;

alter table cb_books
add foreign key (author_id)
references cb_authors;