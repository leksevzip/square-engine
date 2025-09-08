// Copyright 2022 The Manifold Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "manifold/manifold.h"
#include "utils.h"

namespace manifold {

enum class SEONodeType { Union, Intersection, Difference, Leaf };

class SEOLeafNode;

class SEONode : public std::enable_shared_from_this<SEONode> {
 public:
  virtual std::shared_ptr<SEOLeafNode> ToLeafNode() const = 0;
  virtual std::shared_ptr<SEONode> Transform(const mat3x4& m) const = 0;
  virtual SEONodeType GetNodeType() const = 0;

  virtual std::shared_ptr<SEONode> Boolean(
      const std::shared_ptr<SEONode>& second, OpType op);

  std::shared_ptr<SEONode> Translate(const vec3& t) const;
  std::shared_ptr<SEONode> Scale(const vec3& s) const;
  std::shared_ptr<SEONode> Rotate(double xDegrees = 0, double yDegrees = 0,
                                  double zDegrees = 0) const;
};

class SEOLeafNode final : public SEONode {
 public:
  SEOLeafNode();
  SEOLeafNode(std::shared_ptr<const Manifold::Impl> pImpl_);
  SEOLeafNode(std::shared_ptr<const Manifold::Impl> pImpl_, mat3x4 transform_);

  std::shared_ptr<const Manifold::Impl> GetImpl() const;

  std::shared_ptr<SEOLeafNode> ToLeafNode() const override;

  std::shared_ptr<SEONode> Transform(const mat3x4& m) const override;

  SEONodeType GetNodeType() const override;

  static std::shared_ptr<SEOLeafNode> Compose(
      const std::vector<std::shared_ptr<SEOLeafNode>>& nodes);

 private:
  mutable std::shared_ptr<const Manifold::Impl> pImpl_;
  mutable mat3x4 transform_ = la::identity;
};

class SEOOpNode final : public SEONode {
 public:
  SEOOpNode();

  SEOOpNode(const std::vector<std::shared_ptr<SEONode>>& children, OpType op);

  std::shared_ptr<SEONode> Boolean(const std::shared_ptr<SEONode>& second,
                                   OpType op) override;

  std::shared_ptr<SEONode> Transform(const mat3x4& m) const override;

  std::shared_ptr<SEOLeafNode> ToLeafNode() const override;

  SEONodeType GetNodeType() const override;

  ~SEOOpNode();

 private:
  mutable ConcurrentSharedPtr<std::vector<std::shared_ptr<SEONode>>> impl_ =
      ConcurrentSharedPtr<std::vector<std::shared_ptr<SEONode>>>({});
  OpType op_;
  mat3x4 transform_ = la::identity;
  // the following fields are for lazy evaluation, so they are mutable
  mutable std::shared_ptr<SEOLeafNode> cache_ = nullptr;
};

}  // namespace manifold
