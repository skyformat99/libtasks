/*
 * Copyright (c) 2013-2014 Andreas Pohl <apohl79 at gmail.com>
 *
 * This file is part of libtasks.
 *
 * libtasks is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libtasks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libtasks.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#define READ_BUFFER_SIZE_BLOCK 4096

#include <tasks/net/http_base.h>

namespace tasks {
namespace net {

class http_response : public http_base {
  public:
    http_response() : http_base() {}

    inline void set_status(std::string status) {
        m_status = status;
        m_status_code = std::atoi(status.c_str());
    }

    inline const std::string& status() const { return m_status; }

    inline int status_code() const { return m_status_code; }

    void prepare_data_buffer();

    void read_data(net::socket& sock);

    inline bool compressed() const { return m_use_gzip; }

    virtual const char* content_p() const;

    void clear() {
        http_base::clear();
        m_status = "";
        m_line_number = 0;
        m_last_line_start = 0;
        m_content_start = 0;
        m_content_length_exists = false;
        m_chunked_enc = false;
        m_use_gzip = false;
        m_response_string.clear();
        m_transfer_buffer.clear();
    }

  private:
    std::string m_status;
    std::string m_response_string;
    tools::buffer m_transfer_buffer;
    int m_status_code = 0;
    int m_line_number = 0;
    std::size_t m_last_line_start = 0;
    std::size_t m_content_start = 0;
    bool m_content_length_exists = false;
    bool m_chunked_enc = false;
    bool m_use_gzip = false;

    void parse_data();
    void parse_chunked_data();
    void parse_line();
    void parse_status();
    void parse_header();
    void decompress();
};

}  // net
}  // tasks

#endif  // _HTTP_RESPONSE_H_
