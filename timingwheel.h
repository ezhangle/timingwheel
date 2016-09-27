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
  twTimeoutHandler timeoutHandler;
  twPulseLink *link;
  void *data;
};

struct _twNode {
  twNode *prev;
  twNode *next;
  void *data;
};

typedef void (*twTimeoutHandler)(twContext *ctx, twNode *node);

void twInit(twContext *ctx, twPulseLink *pulseLinks, int timeoutPulses);
void twAdd(twContext *ctx, twNode *node, twTimeoutHandler handler);
void twUpdate(twContext *ctx, twNode *node);
void twRemove(twContext *ctx, twNode *node);
void twPulse(twContext *ctx);

#endif
