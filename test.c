#include "timingwheel.h"
#include <stdlib.h>
#include <stdio.h>

#define TIMEOUT_SECONDS 5

twContext gCtx;
twNode gNode;
twNode gNode2;

static void onTimeout(twContext *ctx, twNode *node) {
  printf("id:%d timeout\n", (int)node->data);
  if (1 == (int)node->data) {
    twAdd(&gCtx, &gNode2, onTimeout);
  }
}

int main(int argc, char *argv[]) {
  int pulseCount = 0;
  twPulseLink pulseLinks[TIMEOUT_SECONDS];
  gCtx.data = 0; // user data
  twInit(&gCtx, pulseLinks, TIMEOUT_SECONDS);
  gNode.data = (void *)1;
  twAdd(&gCtx, &gNode, onTimeout);
  gNode2.data = (void *)2;
  twAdd(&gCtx, &gNode2, onTimeout);
  while (pulseCount < 20) {
    ++pulseCount;
    if (3 == pulseCount) {
      twUpdate(&gCtx, &gNode);
      twRemove(&gCtx, &gNode2);
    }
    printf("pulseCount:%d\n", pulseCount);
    twPulse(&gCtx);
  }
  printf("quit\n");
  return 0;
}
