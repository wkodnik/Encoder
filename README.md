# Encoder
Communication Driver Development/Unit Test/Performance Test/
/////////////////////////////////////////////////////////////////////////
////                        can-18F4580.c                             ////
////    can_init - Configures the PIC18F4580 CAN peripheral*         ////
////                                                                 ////
////    can_set_baud - Sets the baud rate control registers*         ////
////                                                                 ////
////    can_set_mode - Sets the CAN module into a specific mode*     ////
////                                                                  ////
////     can_set_functional_mode - Sets the function mode               ////
////                                                                 ////
////    can_set_id - Sets the standard and extended ID*              ////
////                                                                  ////
////     can_set_extended_id - Sets only extended ID                  ////
////                                                                  ////
////     can_set_standard_id - Sets only standard ID                  ////
////                                                                 ////
////    can_get_id - Gets the standard and extended ID*              ////
////                                                                    ////
////     can_get_extended_id - Gets only extended ID                  ////
////                                                                  ////
////     can_get_standard_id - Gets only standard ID                  ////
////                                                                 ////
////    can_putd - Sends a message/request with specified ID*        ////
////                                                                 ////
////    can_getd - Returns specifid message/request and ID*          ////
////                                                                 ////
////    can_kbhit - Returns true if there is data in one of the*     ////
////                receive buffers                                  ////
////                                                                 ////
////    can_tbe - Returns true if the transmit buffer is ready to    ////
////              send more data*                                    ////
////                                                                 ////
////    can_abort - Aborts all pending transmissions*                ////
////                                                                 ////
////    can_enable_b_transfer - enables buffer as transmitter         ////
////                                                                  ////
////     can_enable_b_receiver - enables buffer as receiver             ////
////                                                                  ////
////     can_enable_rtr - enables transmit buffer as Auto-rtr         ////
////                                                                  ////
////     can_disable_rtr - disables transmit buffer as Auto-rtr         ////
////                                                                  ////
////    can_load_rtr - loads an Auto-rtr buffer with data            ////
////                                                                  ////
////    can_enable_filter - enables one of 15 filters                  ////
////                                                                  ////
////    can_disable_filter - disables one of the 15 filters            ////
////                                                                  ////
////    can_associate_filter_to_buffer - associates a filter with    ////
////                                      a receive buffer            ////
////                                                                  ////
////     can_associate_filter_to_mask - associates a filter with a      ////
////                                    mask                           ////
