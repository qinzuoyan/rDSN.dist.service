/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Microsoft Corporation
 * 
 * -=- Robust Distributed System Nucleus (rDSN) -=- 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Description:
 *     What is this file about?
 *
 * Revision history:
 *     xxxx-xx-xx, author, first version
 *     xxxx-xx-xx, author, fix bug about xxx
 */

#pragma once

#include <dsn/dist/failure_detector.h>
#include <dsn/dist/distributed_lock_service.h>
#include "replication_common.h"

namespace dsn { namespace replication {

class meta_service;
namespace test { class test_checker; }
class meta_server_failure_detector : public fd::failure_detector
{
public:
    meta_server_failure_detector(meta_service* svc);
    virtual ~meta_server_failure_detector();

    bool is_primary() const { return _is_primary; }
    rpc_address get_primary();
    
    void acquire_leader_lock();
    void sync_node_state_and_start_service();

    // client side
    virtual void on_master_disconnected(const std::vector<rpc_address>&)
    {
        dassert(false, "unsupported method");
    }

    virtual void on_master_connected(rpc_address)
    {
        dassert(false, "unsupported method");
    }

    // server side
    virtual void on_worker_disconnected(const std::vector< rpc_address>& nodes);
    virtual void on_worker_connected(rpc_address node);

    virtual bool is_worker_connected(rpc_address node) const
    {
        //we treat all nodes not in the worker list alive in the first grace period
        //for those in the worker list, they are surely alive
        if (_election_moment+get_grace_ms() < dsn_now_ms())
        {
            return true;
        }
        return failure_detector::is_worker_connected(node);
    }

    virtual void on_ping(const fd::beacon_msg& beacon, rpc_replier<fd::beacon_ack>& reply);

private:
    void set_primary(rpc_address primary);

private:
    friend class replication_checker;
    friend class test::test_checker;

    meta_service  *_svc;

    zlock         _primary_address_lock;
    volatile bool _is_primary;
    rpc_address   _primary_address;

    dist::distributed_lock_service *_lock_svc;
    task_ptr    _lock_grant_task;
    task_ptr    _lock_expire_task;
    std::string _primary_lock_id;
    std::string _lock_owner_id;
    volatile uint64_t    _election_moment;

public:
    /* these two functions are for test */
    meta_server_failure_detector(rpc_address leader_address, bool is_myself_leader);
    void set_leader_for_test(rpc_address leader_address, bool is_myself_leader);
};

}}

