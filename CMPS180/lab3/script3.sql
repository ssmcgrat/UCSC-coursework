/*
	Sean McGrath, ssmcgrat@ucsc.edu
	CMPS180 Lab Assignment 3
	16 February 2016
*/

-- Edition of a book must be a positive integer 
alter table cb_books
add constraint edition_number check (edition > 0);

-- -------------------------------------------------------------------
-- Replace NULL values for 'address_id' in mg_customers
-- with the value 1
-- -------------------------------------------------------------------

update mg_customers 
set address_id = 1
where address_id is NULL;

-- -------------------------------------------------------------------
-- Add constraint so that customer record may not have an empty value
-- for 'address_id'
-- -------------------------------------------------------------------
alter table mg_customers
add constraint address_id_not_empty 
check (address_id is not NULL);

-- -------------------------------------------------------------------
-- 'dv_address' table may not have an empty value for 'address'
-- -------------------------------------------------------------------
alter table dv_address
add constraint address_not_empty
check (address is not NULL);