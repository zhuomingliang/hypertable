/** -*- c++ -*-
 * Copyright (C) 2011 Hypertable, Inc.
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 3 of the
 * License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef HYPERTABLE_LOADBALANCERBASICDISTRIBUTELOAD_H
#define HYPERTABLE_LOADBALANCERBASICDISTRIBUTELOAD_H

#include <set>
#include <iostream>

#include "Common/String.h"
#include "Hypertable/Lib/Client.h"

#include "RSMetrics.h"

namespace Hypertable {


  class LoadBalancerBasicDistributeLoad {

    public:
      LoadBalancerBasicDistributeLoad(double max_load_deviation, TablePtr &table)
        :  m_loadavg_deviation_threshold(max_load_deviation), m_table(table) { }
      void compute_plan(BalancePlanPtr &balance_plan);

    public:
      class ServerMetricSummary {
      public:
        ServerMetricSummary() { clear(); }
        void clear() { loadavg = loadavg_per_loadestimate = 0; server_id = 0; }

        double loadavg;
        double loadavg_per_loadestimate;
        const char *server_id;
      };
      struct GtServerMetricSummary {
        bool operator() (const ServerMetricSummary &x, const ServerMetricSummary &y) const {
          return x.loadavg > y.loadavg;
        }
      };
      typedef std::multiset<ServerMetricSummary, GtServerMetricSummary> ServerSetDescLoad;

      class RangeMetricSummary {
      public:
        RangeMetricSummary() { clear(); }
        void clear() { loadestimate = 0; table_id = start_row = end_row = 0; }

        double loadestimate;
        const char *table_id;
        const char *start_row;
        const char *end_row;
      };
      struct GtRangeMetricSummary {
        bool operator() (const RangeMetricSummary &x, const RangeMetricSummary &y) const {
          return x.loadestimate > y.loadestimate;
        }
      };
      typedef std::multiset<RangeMetricSummary, GtRangeMetricSummary> RangeSetDescLoad;

    private:
      void calculate_server_summary(const ServerMetrics &metrics, ServerMetricSummary &summary);
      void calculate_range_summary(const RangeMetrics &metrics, RangeMetricSummary &summary);
      void populate_range_load_set(const RangeMetricsMap &range_metrics,
                                   RangeSetDescLoad &ranges_desc_load);
      bool check_move(const ServerMetricSummary &source, const ServerMetricSummary &destination,
                      double range_loadestimate, double mean_loadavg);

      double m_loadavg_deviation_threshold;
      TablePtr &m_table;


  }; // LoadBalancerBasicDistributeLoad

  std::ostream &operator<<(std::ostream &out, const LoadBalancerBasicDistributeLoad::ServerMetricSummary &summary);
  std::ostream &operator<<(std::ostream &out, const LoadBalancerBasicDistributeLoad::RangeMetricSummary &summary);

} // namespace Hypertable

#endif // HYPERTABLE_LOADBALANCERBASICDISTRIBUTELOAD_H
