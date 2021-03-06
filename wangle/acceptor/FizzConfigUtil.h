/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <fizz/server/FizzServerContext.h>
#include <fizz/util/FizzUtil.h>

#include <wangle/acceptor/ServerSocketConfig.h>

namespace wangle {

class FizzConfigUtil {
 public:
  static std::unique_ptr<fizz::server::CertManager>
    createCertManager(const ServerSocketConfig& config);

  static std::shared_ptr<fizz::server::FizzServerContext> createFizzContext(
      const wangle::ServerSocketConfig& config);

  // Creates a TicketCipher with given params
  template <class TicketCipherT>
  static std::unique_ptr<TicketCipherT> createTicketCipher(
      const TLSTicketKeySeeds& seeds,
      std::chrono::seconds validity,
      std::chrono::seconds handshakeValidity,
      folly::Optional<std::string> pskContext) {
    if (seeds.currentSeeds.empty()) {
      return fizz::FizzUtil::createTicketCipher<TicketCipherT>(
          seeds.oldSeeds,
          "",
          seeds.newSeeds,
          validity,
          handshakeValidity,
          std::move(pskContext));
    } else {
      return fizz::FizzUtil::createTicketCipher<TicketCipherT>(
          seeds.oldSeeds,
          seeds.currentSeeds.at(0),
          seeds.newSeeds,
          validity,
          handshakeValidity,
          std::move(pskContext));
    }
  }
};

} // namespace wangle
