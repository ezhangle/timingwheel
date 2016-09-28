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

#ifndef TIMINGWHEEL_H
#define TIMINGWHEEL_H

typedef struct _twContext twContext;
typedef struct _twNode twNode;
typedef struct _twPulseLink twPulseLink;

struct _twPulseLink {
  twNode *firstNode;
  twNode *lastNode;
};

struct _twContext {
  twPulseLink *pulseLinks;
  int timeoutPulses;
  int addPos;
  twPulseLink *link;
  void *data;
};

typedef void (*twTimeoutHandler)(twContext *ctx, twNode *node);

struct _twNode {
  twNode *prev;
  twNode *next;
  twPulseLink *link;
  twTimeoutHandler timeoutHandler;
  void *data;
};

void twInit(twContext *ctx, twPulseLink *pulseLinks, int timeoutPulses);
void twAdd(twContext *ctx, twNode *node, twTimeoutHandler handler);
void twUpdate(twContext *ctx, twNode *node);
void twRemove(twContext *ctx, twNode *node);
void twPulse(twContext *ctx);

#endif
