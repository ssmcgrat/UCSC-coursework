-- -------------------------------------------------------------------
-- This script file will load data from the six local data files 
-- into their respective tables, which are created in script1.sql
-- -------------------------------------------------------------------

\copy DVCustomers FROM dv_customer.data

\copy DVAddresses FROM dv_address.data

\copy DVFilms FROM dv_film.data

\copy CBCustomers FROM cb_customers.data

\copy CBBooks FROM cb_books.data

\copy CBAuthors FROM cb_authors.data
