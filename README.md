# TaurusProject
TaurusProject is the multiplatform programing language project. There will be many elements and progres-steps of this.

Elements:
<ol>
  <li>Terminal</li>
    <p>The Taurus Terminal is a smal deweloper test and it has no specifed features.</p>
    <table>
      <p>List of terminal comands</p>
      <tr>
        <th>ID</th>
        <th>cmd</th>
        <th>args</th>
        <th>description</th>
      </tr>
      <tr>
        <td>0</td>
        <td>exit</td>
        <td>{code:INT}</td>
        <td>calls C exit with 0 or code as arg</td>
      </tr>
      <tr>
        <td>1</td>
        <td>putn</td>
        <td>[n:REL]</td>
        <td>Prints number n</td>
      </tr>
      <tr>
        <td>2</td>
        <td>puts</td>
        <td>[s:STR]</td>
        <td>Prints string s</td>
      </tr>
      <tr>
        <td>3</td>
        <td>push</td>
        <td>[v:ANY]</td>
        <td>Push v to the memory stack</td>
      </tr>
      <tr>
        <td>4</td>
        <td>pop</td>
        <td>[VOID]</td>
        <td>Pop top value from the memory stack</td>
      </tr>
      <tr>
        <td>5</td>
        <td>stack</td>
        <td>{id:INT}</td>
        <td>Gets the top value from the memory stack and puts it to the index of id or _get variable</td>
      </tr>
      <tr>
        <td>6</td>
        <td>size</td>
        <td>{id:INT}</td>
        <td>Returns stack size to id or _get variable</td>
      <tr>
        <td>7</td>
        <td>ipush</td>
        <td>[v:ANY]</td>
        <td>Inits new index with v value and biggest id</td>
      </tr>
      <tr>
        <td>8</td>
        <td>ipop</td>
        <td>[VOID]</td>
        <td>Deletes the biggest index</td>
      </tr>
      <tr>
        <td>9</td>
        <td>index</td>
        <td>[id:INT]</td>
        <td>Gets the value from index id and puts it to _get variable</td>
      </tr>
      <tr>
        <td>10</td>
        <td>index</td>
        <td>[id:INT] [v:ANY]</td>
        <td>Puts the v value to the id index</td>
      </tr>
      <tr>
        <td>11</td>
        <td>isize</td>
        <td>{id:INT}</td>
        <td>Gets size of indexed memory and puts it into index id or _get variable</td>
      </tr>
      <tr>
        <td>12</td>
        <td>iswap</td>
        <td>[id1:INT] [id2:INT]</td>
        <td>Swaps two indexes (index id1 and index id2)</td>
      </tr>
      <tr>
        <td>13</td>
        <td>add</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Adds v1 and v2 and puts result into index id or _get variable</td>
      </tr>
      <tr>
        <td>14</td>
        <td>add</td>
        <td>{id:INT} [v1:STR] [v2:ANY]</td>
        <td>Adds v2 to v1 and puts result into index id or _get variable</td>
      </tr>
      <tr>
        <td>15</td>
        <td>sub</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Substrats v1 and v2 and puts result into index id or _get variable</td>
      </tr>
      <tr>
        <td>16</td>
        <td>sub</td>
        <td>{id:INT} [str:STR] [begin:NUM] {end:NUM}</td>
        <td>Puts the value of str from begin to end or str.length - 1 into index id or _get variable</td>
      </tr>
      <tr>
        <td>17</td>
        <td>mul</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Multibles v1 and v2 and puts result into index id or _get variable</td>
      </tr>
      <tr>
        <td>18</td>
        <td>mul</td>
        <td>{id:INT} [str:STR] [times:NUM]</td>
        <td>Multiples str by times and puts result into index id or _get variable</td>
      </tr>
      <tr>
        <td>19</td>
        <td>div</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Divides v1 by v2 and puts result into index id or _get variable</td>
      </tr>
      <tr>
        <td>20</td>
        <td>mod</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Puts modulo v1 by v2 into index id or _get variable</td>
      </tr>
      <tr>
        <td>21</td>
        <td>typeof</td>
        <td>{id:INT} [v:ANY]</td>
        <td>Returns v's type into index id or _get variable</td>
      </tr>
    </table>
    <table>
      <p>List of predefined variables</p>
      <tr>
        <th>ID</th>
        <th>Name</th>
        <th>Description</th>
      </tr>
      <tr>
        <td>0</td>
        <td>$</td>
        <td>Alias of _get</td>
      </tr>
      <tr>
        <td>1</td>
        <td>_get</td>
        <td>result register</td>
      </tr>
      <tr>
        <td>2</td>
        <td>_top</td>
        <td>referention to the top of memory stack</td>
      </tr>
      <tr>
        <td>3</td>
        <td>_true</td>
        <td>VAR_BOOL true ("1") const variable</td>
      </tr>
      <tr>
        <td>4</td>
        <td>_false</td>
        <td>VAR_BOOL false ("0") const variable</td>
      </tr>
      <tr>
        <td>5</td>
        <td>_int</td>
        <td>VAR_TYPE INT ("0") const variable</td>
      </tr>
      <tr>
        <td>6</td>
        <td>_dot</td>
        <td>VAR_TYPE DOT ("1") const variable</td>
      </tr>
      <tr>
        <td>7</td>
        <td>_str</td>
        <td>VAR_TYPE STR ("2") const variable</td>
      </tr>
      <tr>
        <td>8</td>
        <td>_bool</td>
        <td>VAR_TYPE BOOL ("3") const variable</td>
      </tr>
      <tr>
        <td>9</td>
        <td>_type</td>
        <td>VAR_TYPE TYPE ("4") const variable</td>
      </tr>
    </table>
</ol>
