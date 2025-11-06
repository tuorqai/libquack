
|       |            |              | Notes
|-------|------------|--------------|---------|
| `au_` | namespace  | `audio`      |
| `ab_` | namespace  | `audio`      | backend |
| `co_` | namespace  | `core`       |
| `fs_` | namespace  | `filesystem` |
| `g_`  | namespace  | `graphics`   |
| `log_`| namespace  | `log`        |
| `p_`  | identifier | _pointer_    | when distinguishing from handles |
| `pl_` | namespace  | `platform`   |
| `r_`  | namespace  | `renderer`   | backend for `graphics` |
| `ut_` | namespace  | `util`       |
| `w_`  | namespace  | `window`     | backend for `core` |

```c
void qu_draw_texture(qu_texture texture)
{
    g_texture *p_texture = ut_get_handle_object(texture);

    if (p_texture == NULL) {
        return;
    }

    g_draw_texture(p_texture);
}
```