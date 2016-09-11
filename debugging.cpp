# include "io_service.hpp"
# include "dynamic_buffer.hpp"
# include <boost/cstdint.hpp>

# include <iostream>

void(set_digit_pin_mode (uint8_t (__pin_id ), uint8_t (__pin_mode ) ) ) { }

void(set_digit_pin_state (uint8_t (__pin_id ), uint8_t (__pin_state ) ) ) { }

int unsigned (simulated_pin_state_i [2] [8]) = { {1, 0, 1, 1, 1, 1, 1, 1}, {1, 1, 0, 1, 1, 1, 1, 1} } ;

int unsigned (simulated_pin_state_c [10]) = {1, 0, 1, 1, 1, 1, 1, 1, 1, 1} ;

int unsigned (simulated_return_i ) = 0;
int unsigned (current_pin_pos_i ) = 0;
int unsigned (current_arr_pos ) = 0;

int unsigned (simulated_return_c ) = 0;
int unsigned (current_pin_pos_c ) = 0;

int(get_digit_pin_state (uint8_t (__pin_id ) ) )
{
    if (__pin_id == 2 || __pin_id == 3)
    {
        simulated_return_i = (simulated_pin_state_i [current_arr_pos] [current_pin_pos_i]);

        if (current_pin_pos_i == 7)
        {
            current_pin_pos_i = 0;
            if (current_arr_pos == 1)
                current_arr_pos = 0;
            else
                current_arr_pos ++;
        }
        else current_pin_pos_i ++;
        return simulated_return_i;
    }

    if (__pin_id == 6 )
    {
        simulated_return_c = (simulated_pin_state_c [current_pin_pos_c]);
        if (current_pin_pos_c == 9)
            current_pin_pos_c = 0;
        else
            current_pin_pos_c ++;
        return simulated_return_c;
    } return 0;
}

void(external_mlinit (adtp::io_service (* _this ) ) )
{

}

void(external_mltick (adtp::io_service (* _this ) ) )
{

}

int(main( ) )
{
    int unsigned dbuff_blocks = 2;
    adtp::dbint_t my_dbuff;
    int * block_0_data = new int [2];
    int * block_1_data = new int [2];
    my_dbuff.dbuff_init(1,2,2);

    block_0_data[0] =1;
    block_0_data[1] =2;
    block_1_data[0] =3;
    block_1_data[1] =4;

    for (int a = 0; a!= dbuff_blocks; a++)
        my_dbuff.add_to_dbuff(&block_0_data[a], 2, 0, 0, a, true, true, true);

    for (int b = 0; b!= dbuff_blocks; b++)
        my_dbuff.add_to_dbuff(&block_1_data[b], 2, 0, 1, b, true, true, true);
   
    int * * dbuff_output = new int * [8];
    int xx = 0; 
  
    for (int x = 0; x != dbuff_blocks; x++)
    {
        for (int y = 0; y != dbuff_blocks; y++) 
        {
            //std::cout << ":@" << x+y << std::endl;
            dbuff_output[xx] = my_dbuff.get_from_dbuff(2, 0, x, y, false, false, false, true);
            xx++;
        }
    }
    //dbuff_output[1] = my_dbuff.get_from_dbuff(2, 0, 1, 0, false, false, false, false);
   
    for (int x = 0; x != (dbuff_blocks*dbuff_blocks); x ++)
    {
        if (dbuff_output[x] != nullptr)
        {
            std::cout << x << ">: " << *dbuff_output[x] << std::endl;
        }
        else
            std::cout << "Pointer Is Nulled" << std::endl;
    }
    adtp::io_service io_service (
        & set_digit_pin_mode,
        & set_digit_pin_state,
        & get_digit_pin_state,
        & external_mlinit,
        & external_mltick );
}
