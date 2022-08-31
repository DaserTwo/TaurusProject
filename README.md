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
        <td>calls C exit with 0 or <i>code</i> as arg</td>
      </tr>
      <tr>
        <td>1</td>
        <td>push</td>
        <td>[v:ANY]</td>
        <td>Push <i>v</i> to the memory stack</td>
      </tr>
      <tr>
        <td>2</td>
        <td>pop</td>
        <td>[VOID]</td>
        <td>Pop top value from the memory stack</td>
      </tr>
      <tr>
        <td>3</td>
        <td>stack</td>
        <td>{id:INT}</td>
        <td>Gets the top value from the memory stack and puts it to the index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>4</td>
        <td>size</td>
        <td>{id:INT}</td>
        <td>Returns stack size to index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>5</td>
        <td>ipush</td>
        <td>[v:ANY]</td>
        <td>Inits new index with <i>v</i> value and biggest id</td>
      </tr>
      <tr>
        <td>6</td>
        <td>ipop</td>
        <td>[VOID]</td>
        <td>Deletes the biggest index</td>
      </tr>
      <tr>
        <td>7</td>
        <td>index</td>
        <td>[id:INT]</td>
        <td>Gets the value from index <i>id</i> and puts it to _get variable</td>
      </tr>
      <tr>
        <td>8</td>
        <td>index</td>
        <td>[id:INT] [v:ANY]</td>
        <td>Puts the <i>v</i> value to the index <i>id</i></td>
      </tr>
      <tr>
        <td>9</td>
        <td>isize</td>
        <td>{id:INT}</td>
        <td>Gets size of indexed memory and puts it into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>10</td>
        <td>iswap</td>
        <td>[id1:INT] [id2:INT]</td>
        <td>Swaps two indexes (index <i>id1</i> and index <i>id2</i>)</td>
      </tr>
      <tr>
        <td>11</td>
        <td>add</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Adds <i>v1</i> and <i>v2</i> and puts result into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>12</td>
        <td>add</td>
        <td>{id:INT} [v1:STR] [v2:ANY]</td>
        <td>Adds <i>v2</i> to <i>v1</i> and puts result into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>13</td>
        <td>sub</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Substrats <i>v1</i> and <i>v2</i> and puts result into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>14</td>
        <td>sub</td>
        <td>{id:INT} [str:STR] [begin:NUM] {end:NUM}</td>
        <td>Puts the value of <i>str</i> from <i>begin</i> to <i>end</i> or <i>str.length - 1</i> into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>15</td>
        <td>mul</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Multibles <i>v1</i> and <i>v2</i> and puts result into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>16</td>
        <td>mul</td>
        <td>{id:INT} [str:STR] [times:NUM]</td>
        <td>Multiples <i>str</i> by <i>times</i> and puts result into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>17</td>
        <td>div</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Divides <i>v1</i> by <i>v2</i> and puts result into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>18</td>
        <td>mod</td>
        <td>{id:INT} [v1:REL] [v2:REL]</td>
        <td>Puts modulo <i>v1</i> by <i>v2</i> into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>19</td>
        <td>typeof</td>
        <td>{id:INT} [v:ANY]</td>
        <td>Returns <i>v</i>'s type into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>20</td>
        <td>at</td>
        <td>{id:INT} [s:STR] [index:INT]</td>
        <td>Returns <i>s[index]</i> into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>21</td>
        <td>rel</td>
        <td>{id:INT} [v:ANY]</td>
        <td>Returns <i>v</i> projection to INT or DOT type into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>22</td>
        <td>str</td>
        <td>{id:INT} [v:ANY]</td>
        <td>Returns <i>v</i> projection to STR type into index <i>id</i> or _get variable</td>
      </tr>
      <tr>
        <td>23</td>
        <td>ceil</td>
        <td>{id:INT} [v:DOT]</td>
        <td>Returns round up of <i>v</i> into index <i>id</i> or _get variable as INT type</td>
      </tr>
      <tr>
        <td>24</td>
        <td>floor</td>
        <td>{id:INT} [v:DOT]</td>
        <td>Returns round down of <i>v</i> into index <i>id</i> or _get variable as INT type</td>
      </tr>
      <tr>
        <td>25</td>
        <td>round</td>
        <td>{id:INT} [v:DOT]</td>
        <td>Returns round to nearest of <i>v</i> into index <i>id</i> or _get variable as INT type</td>
      </tr>
      <tr>
        <td>26</td>
        <td>process</td>
        <td>[cmd:STR]</td>
        <td>Processing <i>cmd</i> as standard input</td>
      </tr>
      <tr>
        <td>27</td>
        <td>is</td>
        <td>{id:INT} [v1:ANY] [q:RELATION] [v2:ANY]</td>
        <td>Returns true or false if <i>v1</i> and <i>v2</i> have relation <i>q</i> into index <i>id</i> or _get variable as INT type</td>
      </tr>
      <tr>
        <td>28</td>
        <td>cmp</td>
        <td>{id:INT} [v1:ANY] [v2:ANY]</td>
        <td>Returns relation beetwen <i>v1</i> and <i>v2</i> into index <i>id</i> or _get variable as INT type</td>
      </tr>
      <tr>
        <td>29</td>
        <td>if</td>
        <td>[x:BOOL] [then:STR] {else:STR}</td>
        <td>if <i>x</i> is true then processing <i>then</i> else processing nothing or <i>else</i></td>
      </tr>
      <tr>
        <td>30</td>
        <td>put</td>
        <td>[v:ANY]</td>
        <td>Prints <i>v</i></td>
      </tr>
      <tr>
        <td>31</td>
        <td>putl</td>
        <td>[v:ANY]</td>
        <td>Prints <i>v</i> and new line</td>
      </tr>
      <tr>
        <td>32</td>
        <td>get</td>
        <td>{id:INT}</td>
        <td>Gets keyboard input and saves it into <i>id</i> index or _get</td>
      </tr>
      <tr>
        <td>33</td>
        <td>getl</td>
        <td>{id:INT}</td>
        <td>Gets keyboard input (one line) and saves it into <i>id</i> index or _get</td>
      </tr>
      <tr>
        <td>34</td>
        <td>fput</td>
        <td>[path:STR] [v:ANY]</td>
        <td>Appends <i>v</i> value into <i>path</i> file</td>
      </tr>
      <tr>
        <td>35</td>
        <td>fputl</td>
        <td>[path:STR] [v:ANY]</td>
        <td>Appends <i>v</i> value and new line into <i>path</i> file</td>
      </tr>
      <tr>
        <td>36</td>
        <td>fwrite</td>
        <td>[path:STR] [v:ANY]</td>
        <td>Writes <i>v</i> value into <i>path</i> file</td>
      </tr>
      <tr>
        <td>37</td>
        <td>fwritel</td>
        <td>[path:STR] [v:ANY]</td>
        <td>Writes <i>v</i> value and new line into <i>path</i> file</td>
      </tr>
      <tr>
        <td>38</td>
        <td>fread</td>
        <td>{id:INT} [path:STR]</td>
        <td>Reads all lines from <i>path</i> file and saves them into <i>id</i> index or _get</td>
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
      <tr>
        <td>10</td>
        <td>_relation</td>
        <td>VAR_TYPE RELATION ("5") const variable</td>
      </tr>
      <tr>
        <td>11</td>
        <td>=</td>
        <td>VAR_RELATION equal ("0") const variable</td>
      </tr>
      <tr>
        <td>12</td>
        <td>_equal</td>
        <td>VAR_RELATION equal ("0") const variable</td>
      </tr>
      <tr>
        <td>13</td>
        <td>></td>
        <td>VAR_RELATION bigger ("1") const variable</td>
      </tr>
      <tr>
        <td>14</td>
        <td>_bigger</td>
        <td>VAR_RELATION bigger ("1") const variable</td>
      </tr>
      <tr>
        <td>15</td>
        <td><</td>
        <td>VAR_RELATION smaller ("2") const variable</td>
      </tr>
      <tr>
        <td>16</td>
        <td>_smaller</td>
        <td>VAR_RELATION smaller ("2") const variable</td>
      </tr>
      <tr>
        <td>17</td>
        <td>!</td>
        <td>VAR_RELATION not equal ("3") const variable</td>
      </tr>
      <tr>
        <td>18</td>
        <td>!=</td>
        <td>VAR_RELATION not equal ("3") const variable</td>
      </tr>
      <tr>
        <td>19</td>
        <td>_notequal</td>
        <td>VAR_RELATION not equal ("3") const variable</td>
      </tr>
    </table>
</ol>
