#include "9cc.h"

static void opt(IR *ir) {
  if (ir->op == IR_BPREL) {
    Var *var = ir->var;
    if (var->address_taken || var->ty->ty != INT)
      return;

    if (!var->promoted)
      var->promoted = new_reg();

    ir->op = IR_NOP;
    ir->r0->promoted = var->promoted;
    return;
  }

  if (ir->op == IR_LOAD) {
    if (!ir->r2->promoted)
      return;
    ir->op = IR_MOV;
    ir->r2 = ir->r2->promoted;
    return;
  }

  if (ir->op == IR_STORE) {
    if (!ir->r1->promoted)
      return;
    ir->op = IR_MOV;
    ir->r0 = ir->r1->promoted;
    ir->r1 = NULL;
    return;
  }
}

void optimize(Program *prog) {
  for (int i = 0; i < prog->funcs->len; i++) {
    Function *fn = prog->funcs->data[i];
    for (int i = 0; i < fn->bbs->len; i++) {
      BB *bb = fn->bbs->data[i];
      for (int i = 0; i < bb->ir->len; i++)
        opt(bb->ir->data[i]);
    }
  }
}
