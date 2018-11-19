#include "9cc.h"

static int regno(Reg *r) {
  if (!r)
    return 0;
  if (r->rn != -1)
    return r->rn;
  return r->vn;
}

static char *tostr_call(IR *ir) {
  StringBuilder *sb = new_sb();
  sb_append(sb, format("r%d = %s(", regno(ir->r0), ir->name));
  for (int i = 0; i < ir->nargs; i++) {
    if (i != 0)
      sb_append(sb, ", ");
    sb_append(sb, format("r%d", regno(ir->args[i])));
  }
  sb_append(sb, ")");
  return sb_get(sb);
}
