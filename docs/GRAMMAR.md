## nondigit
    a-z
    A-Z
    _

## digit
    0-9

## dec\_seq
    digit {_} dec\_seq

## sign
    +
    -

## numeric\_frac
    . digit+ [E [sign] digit+]

## bin\_digit
    0
    1

## bin\_seq
    bin\_digit {_} bin\_seq

## bin\_literal
    0b bin\_seq

## oct\_digit
    0-7

## oct\_seq
    oct_digit {_} oct\_seq

## oct\_literal
    0o oct\_seq

## hex\_digit
    0-9
    a-f
    A-F

## hex\_seq
    hex\_digit {_} hex\_seq

## hex\_literal
    0x hex\_seq

## dec\_literal
    dec\_seq
	
## floating_\exponent\_part
	 E [sign] dec\_seq
	 e [sign] dec\_seq
	 P [sign] dec\_seq
	 p [sign] dec\_seq

## numeric\_literal
    dec\_literal [. dec\_seq] [floating\_exponent\_part]
    hex\_literal [. hex\_seq] [floating\_exponent\_part]
    oct\_literal [. oct\_seq] [floating\_exponent\_part]
    bin\_literal [. bin\_seq] [floating\_exponent\_part]

## bool\_literal
    true
    false

## null\_literal
    null

## hex\_escape\_seq
	\x { hex\_digit }
	
## oct\_escape\_seq
	\o { oct\_digit }
	
## bin\_escape\_seq
	\b { bin\_digit }
	
## unicode\_escape\_seq
	\u { hex\_digit }
	
## simple\_escape\_seq
	\0
    \'
    \"
    \\
    \?
    \f
    \n
    \r
    \t
    \  # followed by a space; no character is parsed; to force end a unicode escape sequence

## escape\_seq
    simple_escape_seq
    hex_escape_seq
    oct_escape_seq
    bin_escape_seq
    unicode_escape_seq

## schar
    ?any character not "?
    *escape_seq*
	
## rchar
	?any character not `?

## normal\_string\_literal
    " {schar} "
	
## raw\_string\_literal
	` {rchar} `

## string\_literal\_part
	normal\_string\_literal
	raw\_string\_literal
	
## string\_literal
    {string\_literal\_part $}

## id
    *nondigit* { *digit* | *nondigit* }

## line\_comment
    / / ?any character not \n?

## start\_block\_comment
    / *

## end\_block\_comment
    * /

## block\_comment\_character
    *block\_comment*
    ?any character sequence not *end\_block_comment*

## block\_comment
    *start\_block\_comment* {*block\_comment\_character* $} *end\_block\_comment*

## comment
    *line\_comment*
    *block\_comment*

----------

## module
    {*declaration* $}

----------

## statement\_body
    *compound\_stmt*
    *jump\_stmt*
    *label_stmt*
    *case\_stmt*
    *if\_stmt*
    *loop\_stmt*
    *use\_stmt*
    *assignment\_stmt*
    *expression*
    *declaration*

## statement\_terminator
    ;
    \n

## statement
    *statement\_body* *statement\_terminator*+

## assignment\_op
    =
    :=
    +=
    -=
    *=
    /=
    %=
    >>=
    <<=
    &=
    |=
    ^=

## assignment\_stmt
    *value\_exp* *assignment\_op* $ *expression*

## compound\_stmt
    { $ [*statement* {. *statement*}] $ }

## jump\_stmt
    goto *id*
    break
    continue
    return [*expression*]

## label\_stmt
    label *id*

## else\_block
    else $ *statement*

## if\_stmt
    if ( $ *logical\_exp* $ ) $ *statement* [*else_block*] 

## switch\_stmt
    switch ( $ *logical\_exp* $ ) $ *statement*

## case\_stmt
    case *expression* {, $ *expression*}

## loop\_stmt
    while ( $ [*value\_exp*] $ ) $ *statement* [*else_block*]
    do $ *statement* $ while ( $ *value\_exp* $ ) 
    for ( $ *declaration* ; $ *expression* ;  $ *expression* $ ) $ *statement* [*else_block*]
    foreach ( *declaration* in *expression* ) *statement* [*else_block*]

## use\_stmt
    use *id*

## include\_stmt
    include *id* //allow id.id.id
    include *path*

----------

## type\_qual
    const

## base\_type
    *primative\_type*
    *user\_type*
    *tuple\_type*

## type
    *base\_type* {*type\_postfix*}

## primative\_type\_class
    var
    num
    integer

<!--
## vector\_type
    vchar2
    vchar4
    vchar8
    vchar16
    vshort2
    vshort4
    vshort8
    vint4
-->
    

## primative\_type
    *primate\_type\_class*
<!-- *vector\_type* -->
    bool
    void
    char
    short
    int
    long
    uchar
    ushort
    uint
    ulong
    int8
    int16
    int32
    int64
    uint8
    uint16
    uint32
    uint64
    float
    double
    float32
    float64

## user\_type
    *id*

## type\_list
    *type* {, $ *type*}

## tuple\_type
    [ $ *type\_list* $ ]

## type\_postfix
    function\_type\_postfix [*type\_postfix*]
    pointer\_type\_postfix [*type\_postfix*]
    array\_type\_postfix [*type\_postfix*]

## function\_type\_postfix
    func ( $ [*type\_list*] $ )

## pointer\_type\_postfix
    ^

## array\_type\_postfix
    *static\_array\_type\_postfix*
    *dynamic\_array\_type\_postfix*

## static\_array\_type\_postfix
    [ $ *expression* $ ]

## dynamic\_array\_type\_postfix
    [ $ ]

-----------

## storage\_modifier
    extern
    undecorated
    decorated
    static

## access\_modifier
    public
    private

## type\_modifier
    weak

## declaration
    { *access\_modifier* $ } *base\_declaration*

## base\_declaration
    *type\_decl*
    *nontype\_decl*
    *alias\_decl*

## simple\_template
    *id*
    *basic\_type*

## template\_member
    *type*
    *primary\_exp*

## template\_spec
    ! *template\_member*
    ! [ *template\_member* {, *template\_member*} ]

## type\_decl\_body
    { $ [*declaration* {. *declaration*}] $ }

## type\_decl
    struct [*id*] $ *type\_decl\_body*
    union [*id*] $ *type\_decl\_body*
    class [*id*] [: *id*] $ *type\_decl\_body*
    interface [*id*] $ *type\_decl\_body*

## nontype\_decl
    *function\_decl*
    *variable\_decl*

## parameter
    *type* *id* [= *expression*]

## parameter\_list
    [*parameter* [, *parameter\_list*]]
    ...

## function\_decl
    *type* *id* ( *parameter\_list* ) $ *statement* .

## variable\_decl
    *type\_qual* *type* *id* [= $ *value\_exp*]

## alias\_decl
    let *id* = $ *expression*

-----------

## expression
    *value\_exp*

## primary\_expression
    *literal\_exp*
    *tuple\_exp*
    *paren\_exp*
    this
    super
    *identifier\_exp*

## literal\_exp
    char\_literal
    string\_literal
    bool\_literal
    null\_literal
    numeric\_literal

## tuple\_exp
    [ $ *expression* {, $ *expression*} $ ]

## paren\_exp
    ( $ *expression* $ )

## identifier\_exp
    *id*

## import\_exp
    import *id* {. *id*}

## member\_exp
    . *id*
    *expression* . *id*

## pack\_exp
    pack *value\_exp*

## expression\_list
    *expression* {, $ *expression*}
    
## call\_exp
    *primary\_postfix\_exp* ( $ [*expression\_list*] $ )

## mem\_exp
    new *type* [( $ [*expression\_list*] $ )]
    delete *primary\_postfix\_exp*
    retain *primary\_postfix\_exp*
    release *primary\_postfix\_exp*

## index\_exp
    *posfix\_exp* [ $ *expression* $ ]

## arithmetic\_postifx\_exp
    *postfix\_exp* ++
    *postfix\_exp* --

## primary\_postfix\_exp
    *member\_exp*
    *index\_exp*
    *primary\_exp*
    *call\_exp*

## postfix\_exp
    *arithmetic\_postifx\_exp*
    *primary\_postfix\_exp*

## unary\_exp
    ++ *cast\_exp*
    -- *cast\_exp*
     + *cast\_exp*
     - *cast\_exp*
    ! *cast\_exp*
    ~ *cast\_exp*
    ^ *cast\_exp*
    & *cast\_exp*
    *mem\_exp*
    *pack\_exp*
    *postfix\_exp*

## cast\_exp
    *type* : *cast\_exp*
    *unary\_exp*

## mulplicative\_exp
    *mulplicative\_exp* % $ *value\_exp*
    *mulplicative\_exp* ** $ *value\_exp*
    *mulplicative\_exp* / $ *value\_exp*
    *mulplicative\_exp* * $ *value\_exp*
    *cast\_exp*

## shift\_exp
    *shift\_exp* >> $ *value\_exp*
    *shift\_exp* << $ *value\_exp*
    *mulplicative\_exp*

## additive\_exp
    *additive\_exp* + $ *shift\_exp*
    *additive\_exp* - $ *shift\_exp*
    *shift\_exp*

## bitwise\_exp
    *bitwise\_exp* ^ $ *value\_exp*
    *bitwise\_exp* & $ *value\_exp*
    *bitwise\_exp* | $ *value\_exp*
    *additive\_exp*

## relational\_exp
    *relational\_exp* < $ *value\_exp*
    *relational\_exp* <= $ *value\_exp*
    *relational\_exp* > $ *value\_exp*
    *relational\_exp* >= $ *value\_exp*
    *relational\_exp* != $ *value\_exp*
    *relational\_exp* == $ *value\_exp*
    *bitwise\_exp*

## logical\_exp
    *logical\_exp* or $ *value\_exp*
    *logical\_exp* || $ *value\_exp*
    *logical\_exp* and $ *value\_exp*
    *logical\_exp* && $ *value\_exp*
    *relational\_exp*

## value\_exp
    *logical\_exp*
