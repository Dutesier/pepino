# BNF
```bnf
<gherkin-file> ::= <feature> <scenarios>

<feature> ::= "Feature:" <description> <newline>

<scenarios> ::= (<background> | <scenario> | <scenario-outline>)*
  
<background> ::= "Background:" <newline> <steps>

<scenario> ::= "Scenario:" <description> <newline> <steps>

<scenario-outline> ::= "Scenario Outline:" <description> <newline> <steps> <examples>

<examples> ::= "Examples:" <newline> <example-rows>

<example-rows> ::= <example-header> <newline> <example-data> (<newline> <example-data>)*

<example-header> ::= "|" <identifier> ("|" <identifier>)* "|"

<example-data> ::= "|" <value> ("|" <value>)* "|"

<steps> ::= (<step> <newline>)*

<step> ::= <given> | <when> | <then> | <and>

<given> ::= "Given" <step-description>

<when> ::= "When" <step-description>

<then> ::= "Then" <step-description>

<and> ::= "And" <step-description>

<description> ::= <text> <newline>*

<step-description> ::= <text>

<identifier> ::= <alphanumeric-string>

<value> ::= <alphanumeric-string>

<text> ::= <character>+
<newline> ::= "\n" | "\r\n"

<character> ::= <any UTF-8 character except newline>
<alphanumeric-string> ::= <letter> (<letter> | <digit>)*
<letter> ::= "A" | "B" | "C" | ... | "Z" | "a" | "b" | ... | "z"
<digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
```