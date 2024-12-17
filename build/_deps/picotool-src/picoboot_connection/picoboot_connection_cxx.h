/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICOBOOT_CONNECTION_CXX_H
#define _PICOBOOT_CONNECTION_CXX_H

#include "picoboot_connection.h"
#include <vector>

namespace picoboot {

    struct command_failure : public std::exception {
        explicit command_failure(int code) : code(code) {}

        const char *what() const noexcept override;

        int get_code() const { return code; }
    private:
        int code;
    };

    struct connection_error : public std::exception {
        connection_error(int libusb_code) : libusb_code(libusb_code) {}
        const int libusb_code;
    };

    struct connection {
        explicit connection(libusb_device_handle *device, model_t model, bool exclusive = true) : device(device), model(model), exclusive(exclusive) {
            // do a device reset in case it was left in a bad state
            reset();
            if (exclusive) exclusive_access(EXCLUSIVE);
        }
        ~connection() {
            if (exclusive) {
                if (picoboot_exclusive_access(device, NOT_EXCLUSIVE)) {
                    // failed to restore exclusive access, so just reset
                    reset();
                }
            }
        }
        void reset();
        void exclusive_access(uint8_t exclusive);
        void enter_cmd_xip();
        void exit_xip();
        void reboot(uint32_t pc, uint32_t sp, uint32_t delay_ms);
        void reboot2(struct picoboot_reboot2_cmd *cmd);
        void exec(uint32_t addr);
        // void exec2(struct picoboot_exec2_cmd *cmd); // currently unused
        void get_info(struct picoboot_get_info_cmd *get_info_cmd, uint8_t *buffer, uint32_t len);
        void flash_erase(uint32_t addr, uint32_t len);
        void vector(uint32_t addr);
        void write(uint32_t addr, uint8_t *buffer, uint32_t len);
        void write_bytes(uint32_t addr, std::vector<uint8_t> bytes) {
            write(addr, bytes.data(), bytes.size());
        }
        void read(uint32_t addr, uint8_t *buffer, uint32_t len);
        void otp_write(struct picoboot_otp_cmd *otp_cmd, uint8_t *buffer, uint32_t len);
        void otp_read(struct picoboot_otp_cmd *otp_cmd, uint8_t *buffer, uint32_t len);
        void flash_id(uint64_t &data);

        model_t get_model() const { return model; }
        std::vector<uint8_t> read_bytes(uint32_t addr, uint32_t len) {
            std::vector<uint8_t> bytes(len);
            read(addr, bytes.data(), len);
            return bytes;
        }
    private:
        template <typename F> void wrap_call(F&& func);
        libusb_device_handle *device;
        model_t model;
        bool exclusive;
    };

}

#endif
