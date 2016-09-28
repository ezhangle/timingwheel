/* Copyright (c) huxingyi@msn.com All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "timingwheel.h"

void twInit(twContext *ctx, twPulseLink *pulseLinks, int timeoutPulses) {
  int i;
  ctx->pulseLinks = pulseLinks;
  ctx->timeoutPulses = timeoutPulses;
  for (i = 0; i < timeoutPulses; ++i) {
    twPulseLink *link = &ctx->pulseLinks[i];
    link->firstNode = 0;
    link->lastNode = 0;
  }
  ctx->addPos = 0;
}

static void addNodeToLink(twNode *node, twPulseLink *link) {
  if (link->lastNode) {
    link->lastNode->next = node;
  } else {
    link->firstNode = node;
  }
  node->prev = link->lastNode;
  node->next = 0;
  node->link = link;
  link->lastNode = node;
}

static void removeNodeFromLink(twNode *node, twPulseLink *link) {
  if (node->next) {
    node->next->prev = node->prev;
  } else {
    link->lastNode = node->prev;
  }
  if (node->prev) {
    node->prev->next = node->next;
  } else {
    link->firstNode = node->next;
  }
  node->link = 0;
}

void twAdd(twContext *ctx, twNode *node, twTimeoutHandler handler) {
  addNodeToLink(node, &ctx->pulseLinks[ctx->addPos]);
  node->timeoutHandler = handler;
}

void twUpdate(twContext *ctx, twNode *node) {
  removeNodeFromLink(node, node->link);
  addNodeToLink(node, &ctx->pulseLinks[ctx->addPos]);
}

void twRemove(twContext *ctx, twNode *node) {
  removeNodeFromLink(node, node->link);
}

void twPulse(twContext *ctx) {
  twPulseLink timeoutLink = {0};
  twPulseLink *link;
  int timeoutPos;
  timeoutPos = (ctx->addPos + 1) % ctx->timeoutPulses;
  link = &ctx->pulseLinks[timeoutPos];
  while (link->firstNode) {
    twNode *node = link->firstNode;
    removeNodeFromLink(node, node->link);
    addNodeToLink(node, &timeoutLink);
  }
  while (timeoutLink.firstNode) {
    twNode *node = timeoutLink.firstNode;
    removeNodeFromLink(node, node->link);
    node->timeoutHandler(ctx, node);
  }
  ctx->addPos = timeoutPos;
}
