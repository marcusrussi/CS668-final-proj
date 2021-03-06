
#ifndef _DB_APPLICATIONS_TPCC_UTILS_H_
#define _DB_APPLICATIONS_TPCC_UTILS_H_


//------------------------------------------------------------------------------------
  class TPCCUtil {
    private:
        static const int OL_I_ID_C = 7911; // in range [0, 8191]
        static const int C_ID_C = 259; // in range [0, 1023]
        // NOTE: TPC-C 2.1.6.1 specifies that abs(C_LAST_LOAD_C - C_LAST_RUN_C) must
        // be within [65, 119]
        static const int C_LAST_LOAD_C = 157; // in range [0, 255]
        static const int C_LAST_RUN_C = 223; // in range [0, 255]

        //uint32_t m_seed;

         void
        gen_last_name(int num, char *buf) const {
            static const char *name_tokens[] = {"BAR", "OUGHT", "ABLE", "PRI", 
                                                "PRES", "ESE", "ANTI", "CALLY", 
                                                "ATION", "EING" };
            static const int token_lengths[] = {3, 5, 4, 3, 4, 3, 5, 5, 4};

            int indices[] = { num/100, (num/10)%10, num%10 };
            int offset = 0;

            for (uint32_t i = 0; i < sizeof(indices)/sizeof(*indices); ++i) {
                memcpy(buf+offset, name_tokens[indices[i]], 
                       token_lengths[indices[i]]);
                offset += token_lengths[indices[i]];
            }
            buf[offset] = '\0';
        }

        int
        gen_non_uniform_rand(int A, int C, int min, int max) const {
            int range = max - min + 1;
            int diff = ((gen_rand_range(0, A) | gen_rand_range(min, max)) + C) % range;
            return diff;
        }
    
    public:
        TPCCUtil() {
          //m_seed = time(NULL);
        }

        void
        gen_last_name_load(char *buf) const {
            gen_last_name(gen_non_uniform_rand(255, C_LAST_LOAD_C, 0, 999), buf);
        }

        void
        gen_last_name_run(char *buf) const {
            gen_last_name(gen_non_uniform_rand(255, C_LAST_RUN_C, 0, 999), buf);
        }
        
        int
        gen_rand_range(int min, int max) const {
            int range = max - min + 1;
            return min + (rand() % range);
        }    

        void
        gen_rand_string(int min, int max, char *buf) const {
            int ch_first = (int)'a', ch_last = (int)'z';
            int length = gen_rand_range(min, max);
            for (int i = 0; i < length; ++i) {
                buf[i] = (char)gen_rand_range(ch_first, ch_last);
            }
            buf[length] = '\0';        
        }
    
         void
        append_strings(char *dest, const char **sources, int dest_len, 
                       int num_sources) const {
            int offset = 0;
            for (int i = 0; i < num_sources; ++i) {
                strcpy(dest+offset, sources[i]);
                offset += strlen(sources[i]);
            }
            dest[offset] = '\0';
        }
    };

//-----------------------------------------------------------------------

enum TPCCTable {
  WAREHOUSE = 0,
  DISTRICT = 1,
  CUSTOMER = 2,
  HISTORY = 3,
  NEW_ORDER = 4,
  ORDER = 5,
  ORDER_LINE = 6,
  ITEM = 7,
  STOCK = 8,
};


struct Warehouse {
  uint32_t w_id; // primary key
  char w_name[11];
  char w_street_1[21];
  char w_street_2[21];
  char w_city[21];
  char w_state[3];
  char w_zip[10];
  float w_tax;
  float w_ytd;
};

struct District {
  uint32_t d_id;
  uint32_t d_w_id;
  char d_name[11];
  char d_street_1[21];
  char d_street_2[21];
  char d_city[21];
  char d_state[3];
  char d_zip[10];
  float d_tax;
  float d_ytd;
  uint32_t d_next_o_id;

};

struct Customer {
  uint32_t c_id;
  uint32_t c_d_id;
  uint32_t c_w_id;
  char c_first[17];
  char c_middle[3];
  char c_last[17];
  char c_street_1[21];
  char c_street_2[21];
  char c_city[21];
  char c_state[3];
  char c_zip[10];
  char c_phone[17];
  uint64_t c_since;
  char c_credit[3];
  float c_credit_lim;
  float c_discount;
  float c_balance;
  float c_ytd_payment;
  uint32_t c_payment_cnt;
  uint32_t c_delivery_cnt;
  char c_data[501];
};

struct History {
  uint32_t h_c_id;
  uint32_t h_c_d_id;
  uint32_t h_c_w_id;
  uint32_t h_d_id;
  uint32_t h_w_id;
  uint64_t h_date;
  float h_amount;
  char h_data[25];
};

struct NewOrder {
  uint32_t no_o_id;
  uint32_t no_d_id;
  uint32_t no_w_id;
};

struct Order {
  uint32_t o_id;
  uint32_t o_d_id;
  uint32_t o_w_id;
  uint32_t o_c_id;
  uint64_t o_entry_d;
  uint32_t o_carrier_id;
  uint32_t o_ol_cnt;
  uint32_t o_all_local;
};

struct OrderLine {
  uint32_t ol_o_id;
  uint32_t ol_d_id;
  uint32_t ol_w_id;
  uint32_t ol_number;
  uint32_t ol_i_id;
  uint32_t ol_supply_w_id;
  uint64_t ol_delivery_d;
  uint32_t ol_quantity;
  float ol_amount;
  char ol_dist_info[25];
};

struct Item {
  uint32_t i_id;
  uint32_t i_im_id;
  char i_name[25];
  float i_price;
  char i_data[51];
};

struct Stock {
  uint32_t s_i_id;
  uint32_t s_w_id;
  uint32_t s_quantity;
  char s_dist_01[25];
  char s_dist_02[25];
  char s_dist_03[25];
  char s_dist_04[25];
  char s_dist_05[25];
  char s_dist_06[25];
  char s_dist_07[25];
  char s_dist_08[25];
  char s_dist_09[25];
  char s_dist_10[25];
  float s_ytd;
  uint32_t s_order_cnt;
  uint32_t s_remote_cnt;
  char s_data[51];
};

//----------------------------------------------
  class TPCCKeyGen {
    private:
        
        static const uint32_t s_district_shift = 		12;
        static const uint32_t s_customer_shift = 		16;
        static const uint32_t s_new_order_shift = 		16;
        static const uint32_t s_order_shift = 			16;
        static const uint32_t s_order_line_shift = 		56;
        static const uint32_t s_stock_shift = 			28;

        static const uint64_t s_warehouse_mask = 		0x0000000000000FFF;
        static const uint64_t s_district_mask = 		0x000000000000F000;
        static const uint64_t s_customer_mask = 		0x000000000FFF0000;
        static const uint64_t s_stock_mask = 			0x0000FFFFF0000000;    
        static const uint64_t s_order_mask = 			0x00FFFFFFFFFF0000;

    public:

        // Expects: 	warehouse_id 	==> 	keys[0]
        //			district_id 	==> 	keys[1]
        static inline uint64_t
        create_district_key(uint32_t *keys) {
            return (
                    ((uint64_t)keys[0])				|
                    ((uint64_t)keys[1] << s_district_shift)
                    );
        }

        // Expects: 	warehouse_id 	==> 	keys[0] 
        // 				district_id  	==> 	keys[1]
        //				customer_id  	==> 	keys[2]
        static inline uint64_t
        create_customer_key(uint32_t *keys) {
            return (
                    ((uint64_t)keys[0])				|
                    (((uint64_t)keys[1]) << s_district_shift)		|
                    (((uint64_t)keys[2]) << s_customer_shift)		
                    );
        }


        // Expects: 	warehouse_id 	==> 	keys[0]
        // 			district_id 	==> 	keys[1]
        //			new_order_id	==> 	keys[2]
        static inline uint64_t
        create_new_order_key(uint32_t *keys) {
            return (
                    ((uint64_t)keys[0])    				|
                    ((uint64_t)keys[1] << s_district_shift)		|
                    ((uint64_t)keys[2] << s_new_order_shift)
                    );
        }	

        // Expects: 	warehouse_id 	==> 	keys[0]
        // 			district_id 	==> 	keys[1]
        //			order_id	==> 	keys[2]
        static inline uint64_t
        create_order_key(uint32_t *keys) {
            return (
                    ((uint64_t)keys[0])				|
                    (((uint64_t)keys[1]) << s_district_shift)		|
                    (((uint64_t)keys[2]) << s_order_shift)
                    );
        }

        static inline uint64_t
        create_order_line_key(uint32_t *keys) {            
            return (
                    ((uint64_t)keys[0]) 				| 
                    (((uint64_t)keys[1]) << s_district_shift) 	| 
                    (((uint64_t)keys[2]) << s_order_shift)		|
                    (((uint64_t)keys[3]) << s_order_line_shift)
                    );
        }

        static inline uint64_t
        create_stock_key(uint32_t *keys) {
            return (
                    ((uint64_t)keys[0])				|
                    (((uint64_t)keys[1]) << s_stock_shift)		
                    );
        }

        static inline uint32_t
        get_stock_key(uint64_t composite_key) {
            return (uint32_t)((composite_key & s_stock_mask) >> s_stock_shift);
        }

        static inline uint32_t
        get_customer_key(uint64_t composite_key) {
            return (uint32_t)((composite_key & s_customer_mask) >> s_customer_shift);
        }

        static inline uint32_t
        get_warehouse_key(uint64_t composite_key) {
            return (uint32_t)(composite_key & s_warehouse_mask);
        }

        static inline uint32_t
        get_district_key(uint64_t composite_key) {
            return (uint32_t)((composite_key & s_district_mask) >> s_district_shift);
        }

        static inline uint32_t
        get_order_key(uint64_t composite_key) {
            return (uint32_t)((composite_key & s_order_mask) >> s_order_shift);
        }

    };

#endif  // _DB_APPLICATIONS_TPCC_UTILS_H_
