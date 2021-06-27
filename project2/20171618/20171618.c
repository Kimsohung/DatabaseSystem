#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "kimsohung";
const char* pw = "rlathgmd123";
const char* db = "project3";

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char buffer[1000] = { 0 };
	FILE* fp;
	fopen_s(&fp, "20171618.txt", "r");
	int i = 0;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed, wating for creating database\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}


		while (feof(fp) == 0) {
			while (feof(fp) == 0) {
				buffer[i] = fgetc(fp);
				//printf("%c", buffer[i]);
				if (buffer[i] == ';') {
					//sql Àü¼Û
					const char* query = buffer;
					int state = 0;

					state = mysql_query(connection, query);

					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						mysql_free_result(sql_result);
					}
					for (int j = 0; j <= i; j++) {
						buffer[j] = '\0';
					}
					i = 0;
					break;
				}
				i++;
			}
		}
		int input;
		int inner_input;
		int quit_flag = 0;
		int k;
		int state = 0;
		char c;
		char from_date[40];
		char to_date[40];
		char supplier_name[20];
		char temp_buf1[2048] = { 0 };
		char temp_buf[2048] = { 0 };
		char name[20] = { 0 };
		char temp_buf6[2048] = { 0 };

		while (quit_flag == 0) {
			//system("cls");
			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t6. TYPE 6\n");
			printf("\t7. TYPE 7\n");
			printf("\t0. QUIT\n");
			printf("----------------------------------\n");
			printf("Input query number: ");
			scanf("%d", &input);

			switch (input) {
			case 1:

				//system("cls");
				printf("---- TYPE 1 ----\n\n");
				printf("** Show the sales trends for a particular brand over the past k years. **\n");
				printf(" Which K? : ");
				scanf("%d", &k);
				printf(" Which brand name? : ");
				scanf("%s", name);
				sprintf(temp_buf, "select Models.brand_id, Brands.name, Models.name, Sales.purchased_at from Brands, Models, Sales, Vehicles where(Brands.id = Models.brand_id) and (Models.id = Vehicles.model_id) and (Vehicles.VIN = Sales.VIN) and (Brands.name = '%s') and (YEAR(Sales.purchased_at)) between 2022 - '%d' and 2021; ", name, k);

				state = 0;
				state = mysql_query(connection, temp_buf);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					printf("\n\n");
					printf("%s\t%s\t%s\t%s", "brand_id", "brand_name", "model_name", "purchased_date\n");
					printf("----------------------------------------------------------------------\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\t\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}

				printf("---- TYPE 1-1 ----\n\n");
				printf("** Then break these data out by gender of the buyer. **\n");
				printf("you want to see? if yes press 1 or not, press other number: ");
				scanf("%d", &inner_input);

				if (inner_input == 1) {
					sprintf(temp_buf, "select Models.brand_id, Brands.name, Models.name, Sales.purchased_at, Sales.customer_id, Customers.name, Customers.gender from Brands, Models, Sales, Vehicles, Customers where (Brands.id = Models.brand_id) and (Models.id = Vehicles.model_id) and (Vehicles.VIN = Sales.VIN) and (Brands.name = '%s') and (YEAR(Sales.purchased_at) between 2022 - '%d' and 2021) and (Customers.id = Sales.customer_id) order by YEAR(Sales.purchased_at), Customers.gender; ", name, k);
					state = 0;
					state = mysql_query(connection, temp_buf);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						printf("\n\n");
						printf("%s\t%s\t%s\t%s\t\t%s\t%s\t%s\n", "brand_id", "brand_name", "model_name", "purchased_date", "customer_id", "customer_name", "gender");
						printf("--------------------------------------------------------------------------------------------------------------------\n");
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s\t\t%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6]);
						}
						printf("\n\n");
						mysql_free_result(sql_result);
					}

					printf("---- TYPE 1-1-1 ----\n\n");
					printf("** Then by income range. **\n");
					printf("you want to see? if yes press 1 or not, press other number : ");
					scanf("%d", &inner_input);
					if (inner_input == 1) {
						sprintf(temp_buf, "select Models.brand_id, Brands.name, Models.name, Sales.purchased_at, Sales.customer_id, Customers.name, Customers.gender, Customers.annual_income from Brands, Models, Sales, Vehicles, Customers where (Brands.id = Models.brand_id) and (Models.id = Vehicles.model_id) and (Vehicles.VIN = Sales.VIN) and (Brands.name = '%s') and (YEAR(Sales.purchased_at) between 2022 - '%d' and 2021) and (Customers.id = Sales.customer_id) order by YEAR(Sales.purchased_at), Customers.gender, Customers.annual_income DESC; ", name, k);
						state = 0;
						state = mysql_query(connection, temp_buf);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							printf("\n\n");
							printf("%s\t%s\t%s\t%s\t\t%s\t%s\t%s\t%s\n", "brand_id", "brand_name", "model_name", "purchased_date", "customer_id", "customer_name", "gender", "income");
							printf("--------------------------------------------------------------------------------------------------------------------------\n");
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s\t\t%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7]);
							}
							printf("\n\n");
							mysql_free_result(sql_result);
						}
					}
					else break;

				}
				else break;

				break;

			case 2:
				//system("cls");
				printf("---- TYPE 2 ----\n\n");
				printf("** Show sales trends for various brands over the past k months. **\n");
				printf(" Which K? : ");
				scanf("%d", &k);
				sprintf(temp_buf6, "select Models.brand_id, Brands.name, Models.name, Sales.purchased_at from Brands, Models, Sales, Vehicles where (Brands.id = Models.brand_id) and (Models.id = Vehicles.model_id) and (Vehicles.VIN = Sales.VIN) and (Sales.purchased_at between(select FROM_UNIXTIME(1623542400 - 2629743 * '%d')) and (select FROM_UNIXTIME(1623542400))) order by Brands.id, Sales.purchased_at;", k);

				state = 0;
				state = mysql_query(connection, temp_buf6);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					printf("\n\n");
					printf("%s\t%s\t%s\t%s", "brand_id", "brand_name", "model_name", "purchased_date\n");
					printf("----------------------------------------------------------------------\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\t\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}

				printf("---- TYPE 2-1 ----\n\n");
				printf("** Then break these data out by gender of the buyer. **\n");
				printf("you want to see? if yes press 1 or not, press other number: ");
				scanf("%d", &inner_input);

				if (inner_input == 1) {
					printf("helllo\n");
					sprintf(temp_buf6, "select Models.brand_id, Brands.name, Models.name, Sales.purchased_at, Customers.id, Customers.name, Customers.gender from Brands, Models, Sales, Vehicles, Customers where(Brands.id = Models.brand_id) and (Models.id = Vehicles.model_id) and (Customers.id = Sales.customer_id) and (Vehicles.VIN = Sales.VIN) and (Sales.purchased_at between(select FROM_UNIXTIME(1623542400 - 2629743 * '%d')) and (select FROM_UNIXTIME(1623542400))) order by Brands.id, Sales.purchased_at, Customers.gender;", k);
					state = 0;
					state = mysql_query(connection, temp_buf6);
					if (state == 0)
					{
						printf("bye\n");
						sql_result = mysql_store_result(connection);
						printf("\n\n");
						printf("%s\t%s\t%s\t%s\t\t%s\t%s\t%s\n", "brand_id", "brand_name", "model_name", "purchased_date", "customer_id", "customer_name", "gender");
						printf("--------------------------------------------------------------------------------------------------------------------\n");
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s\t\t%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6]);
						}
						printf("\n\n");
						mysql_free_result(sql_result);
					}

					printf("---- TYPE 2-1-1 ----\n\n");
					printf("** Then by income range. **\n");
					printf("you want to see? if yes press 1 or not, press other number : ");
					scanf("%d", &inner_input);
					if (inner_input == 1) {
						sprintf(temp_buf6, "select Models.brand_id, Brands.name, Models.name, Sales.purchased_at, Customers.id, Customers.name, Customers.gender, Customers.annual_income from Brands, Models, Sales, Vehicles, Customers where (Brands.id = Models.brand_id) and (Models.id = Vehicles.model_id) and (Customers.id = Sales.customer_id) and (Vehicles.VIN = Sales.VIN) and (Sales.purchased_at between(select FROM_UNIXTIME(1623542400 - 2629743 * '%d')) and (select FROM_UNIXTIME(1623542400))) order by Brands.id, Sales.purchased_at, Customers.gender, Customers.annual_income;", k);
						state = 0;
						state = mysql_query(connection, temp_buf6);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							printf("\n\n");
							printf("%s\t%s\t%s\t%s\t\t%s\t%s\t%s\t%s\n", "brand_id", "brand_name", "model_name", "purchased_date", "customer_id", "customer_name", "gender", "income");
							printf("--------------------------------------------------------------------------------------------------------------------------\n");
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("%s\t\t%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7]);
							}
							printf("\n\n");
							mysql_free_result(sql_result);
						}
					}
					else break;

				}
				else break;

				break;

			case 3:
				//system("cls");

				printf("---- TYPE 3 ----\n\n");
				printf("** Find that transmissions made by supplier (company name) between two given dates are defective **\n");
				printf("\t1. TYPE 3-1\n");
				printf("\t2. TYPE 3-2\n");
				printf("Which type? 1 or 2 : ");
				scanf("%d", &inner_input);
				if (inner_input == 1) {
					printf("---- TYPE 3-1 ----\n\n");
					printf("** Find the VIN of each car containing such a transmission and the customer to which it was sold. **\n");
					printf("input from dates (yyyy-mm-dd): ");
					scanf("%s", from_date);
					printf("input to dates (yyyy-mm-dd): ");
					scanf("%s", to_date);
					printf("Which supplier name? : ");
					scanf("%s", supplier_name);
					sprintf(temp_buf1, "select Supply_from_supplier.id as supply_id, Suppliers.name, Supply_from_supplier.supplied_at, Sales.VIN, Sales.customer_id, Sales.purchased_at from Supply_from_supplier, Suppliers, Models, Vehicles, Sales where(Models.id = Supply_from_supplier.model_id) and (Suppliers.name = '%s') and (Supply_from_supplier.supplied_at between '%s' and '%s') and (Models.id = Vehicles.model_id) and (Vehicles.VIN = Sales.VIN) order by Supply_from_supplier.supplied_at, Sales.customer_id; ", supplier_name, from_date, to_date);
					state = 0;
					state = mysql_query(connection, temp_buf1);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						printf("\n\n");
						printf("%s\t%s\t%s\t\t%s\t\t%s\t%s\n", "supply_id", "supplier_name", "supplied_date", "VIN", "customer_id", "purchased_date");
						printf("--------------------------------------------------------------------------------------------------------------------------\n");
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
						}
						printf("\n\n");
						mysql_free_result(sql_result);
					}
				}
				else if (inner_input == 2) {
					printf("---- TYPE 3-2 ----\n\n");
					printf("**  Find the dealer who sold the VIN and transmission for each vehicle containing these transmissions. **\n");
					printf("input from dates (yyyy-mm-dd): ");
					scanf("%s", from_date);
					printf("input to dates (yyyy-mm-dd): ");
					scanf("%s", to_date);
					printf("Which supplier name? : ");
					scanf("%s", supplier_name);
					sprintf(temp_buf1, "select Supply_from_supplier.id as supply_id, Suppliers.name, Supply_from_supplier.supplied_at, Sales.VIN, Sales.dealer_id, Sales.purchased_at from Supply_from_supplier, Suppliers, Models, Vehicles, Sales where(Models.id = Supply_from_supplier.model_id) and (Suppliers.name = '%s') and (Supply_from_supplier.supplied_at between '%s' and '%s') and (Models.id = Vehicles.model_id) and (Vehicles.VIN = Sales.VIN) order by Supply_from_supplier.supplied_at, Sales.dealer_id; ", supplier_name, from_date, to_date);
					state = 0;
					state = mysql_query(connection, temp_buf1);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						printf("\n\n");
						printf("%s\t%s\t%s\t\t%s\t\t%s\t%s\n", "supply_id", "supplier_name", "supplied_date", "VIN", "dealer_id", "purchased_date");
						printf("--------------------------------------------------------------------------------------------------------------------------\n");
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s\t\t%s\t\t%s\t%s\t\t%s\t\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
						}
						printf("\n\n");
						mysql_free_result(sql_result);
					}
				}
				else {
					break;
				}

				break;

			case 4:
				//system("cls");
				printf("---- TYPE 4 ----\n\n");
				printf("** Find the top k brands by dollar-amount sold by the year. **\n");
				printf(" Which K? : ");
				scanf("%d", &k);
				char temp_buf3[400];
				sprintf(temp_buf3, "SELECT t.brand_id, t.name, t.y, t.total_dollar FROM t LEFT JOIN t AS l ON t.y = l.y AND t.total_dollar < l.total_dollar GROUP BY t.y, t.total_dollar HAVING COUNT(l.total_dollar) < '%d' ORDER BY t.y, t.total_dollar DESC; ", k);
				state = 0;

				state = mysql_query(connection, temp_buf3);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					printf("\n\n%s\t%s\t\t%s\t%s\n", "brand_id", "name", "year", "total_dollar");
					printf("---------------------------------------------------------------------\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\t\t%s\t\t%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}

				break;

			case 5:
				//system("cls");
				printf("---- TYPE 5 ----\n\n");
				printf("** Find the top k brands by unit sales by the year**\n");
				printf(" Which K? : ");
				scanf("%d", &k);
				char temp_buf4[400];
				sprintf(temp_buf4, "SELECT brand_id, name, y, unit_sales FROM( SELECT*, RANK() OVER(PARTITION BY y ORDER BY unit_sales DESC) AS rnk FROM s) AS x WHERE rnk <= '%d'; ", k);
				state = 0;

				state = mysql_query(connection, temp_buf4);
				printf("\n\n%s\t%s\t\t%s\t%s\n", "brand_id", "name", "year", "unit_sales");
				printf("---------------------------------------------------------------------\n");
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\t\t%s\t\t%s\t%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}
				break;

			case 6:
				//system("cls");
				printf("---- TYPE 6 ----\n\n");
				printf("** In what month(s) do convertibles sell best?**\n");
				const char* query1 = "select MONTH(Sales.purchased_at), COUNT(Vehicles.VIN) as purchased_convertible_count from Models, Vehicles, Sales where(Models.id = Vehicles.model_id) and (Vehicles.VIN = Sales.VIN) and (Models.vehicle_type = 'convertible') group by MONTH(Sales.purchased_at) order by purchased_convertible_count DESC limit 1;";
				state = 0;

				state = mysql_query(connection, query1);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					printf("\n%s\t%s\n", "month", "selled_convertible_unit");
					printf("-------------------------------\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\t%s\n", sql_row[0], sql_row[1]);
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}
				break;

			case 7:
				//system("cls");
				printf("---- TYPE 7 ----\n\n");
				printf("** Find those dealers who keep a vehicle in inventory for the longest average time.**\n");
				const char* query2 = "select Sales.dealer_id, Dealers.name, avg(Sales.purchased_at - Sales.receipt_at) as inventory_avg_time from Sales, Dealers where(Sales.dealer_id = Dealers.id) group by Sales.dealer_id order by inventory_avg_time DESC limit 1 ";
				state = 0;

				state = mysql_query(connection, query2);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					printf("\n%s\t%s\n", "dealer_id", "dealer_name");
					printf("-------------------------------\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s\t\t%s\n", sql_row[0], sql_row[1]);
					}
					printf("\n\n");
					mysql_free_result(sql_result);
				}
				break;

			case 0:
				quit_flag = 1;
				break;

			default:
				printf("please input correct number\n");
				break;
			}
		}

		//delete and drop 
			//sales
		for (i = 1; i <= 21; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Sales where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c7 = "drop table Sales";
		state = 0;

		state = mysql_query(connection, c7);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//vehicles
		for (i = 10001; i <= 10021; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Vehicles where VIN = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c4 = "drop table Vehicles";
		state = 0;

		state = mysql_query(connection, c4);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//customers
		for (i = 101; i <= 115; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Customers where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c5 = "drop table Customers";
		state = 0;

		state = mysql_query(connection, c5);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//dealers
		for (i = 1; i <= 15; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Dealers where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c6 = "drop table Dealers";
		state = 0;

		state = mysql_query(connection, c6);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//options
		for (i = 1; i <= 21; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Options where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c3 = "drop table Options";
		state = 0;

		state = mysql_query(connection, c3);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//supply_from_supplier
		for (i = 1; i <= 15; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Supply_from_supplier where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c9 = "drop table supply_from_supplier";
		state = 0;

		state = mysql_query(connection, c9);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//supply_from_plant
		for (i = 1; i <= 15; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Supply_from_plant where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c11 = "drop table supply_from_plant";
		state = 0;

		state = mysql_query(connection, c11);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//models
		for (i = 1001; i <= 1015; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Models where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c2 = "drop table Models";
		state = 0;

		state = mysql_query(connection, c2);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//brands
		for (i = 1; i <= 15; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Brands where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c1 = "drop table Brands";
		state = 0;

		state = mysql_query(connection, c1);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//suppliers
		for (i = 1; i <= 15; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Suppliers where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c8 = "drop table Suppliers";
		state = 0;

		state = mysql_query(connection, c8);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		//manufacturing_plants
		for (i = 1; i <= 15; i++) {
			char delete_buf[400];
			sprintf(delete_buf, "delete from Manufacturing_plants where id = '%d'", i);
			state = 0;

			state = mysql_query(connection, delete_buf);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				mysql_free_result(sql_result);
			}
		}
		const char* c10 = "drop table Manufacturing_plants";
		state = 0;

		state = mysql_query(connection, c10);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		const char* c12 = "drop view s";
		state = 0;

		state = mysql_query(connection, c12);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}

		const char* c13 = "drop view t";
		state = 0;

		state = mysql_query(connection, c13);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			mysql_free_result(sql_result);
		}
		mysql_close(connection);
	}

	fclose(fp);
	return 0;
}