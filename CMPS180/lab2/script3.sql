-- -------------------------------------------------------------------
-- Need to preserve the customer_id of the dv_customers table. First
-- copy data from this table into the merged table
-- -------------------------------------------------------------------
/*
INSERT INTO MGCustomers (customer_id, first_name, last_name,
                          email, address_id, active)
*/
INSERT INTO MGCustomers (customer_id)
SELECT DVCustomers.customer_id, DVCustomers.first_name,
       DVCustomers.last_name, DVCustomers.email, DVCustomers.address_id,
	   DVCustomers.active
FROM DVCustomers;

