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
  if (ctx->addPos + 1 < ctx->timeoutPulses) {
    ++ctx->addPos;
  } else {
    ctx->addPos = 0;
  }
  timeoutPos = (ctx->addPos + ctx->timeoutPulses - 1) % ctx->timeoutPulses;
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
}
