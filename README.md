# mdparser

A markdown parser built from scratch in C++. Produces a two-level AST (block tree + inline tree) from a markdown source file, with correct recursive nesting for inline formatting.

## Architecture

The parser runs in three sequential passes:

```
Source file → Tokenizer → Block Parser → Inline Parser → AST
```

### Tokenizer (`tokenizer.h / tokenizer.cpp`)
Reads the raw source string character by character and emits a flat list of typed `Token` structs. Each token carries a `type` (from the `TokenType` enum), a `value` string, and an `index` tracking its position within its line. The index resets to 0 at each newline, which is used downstream to detect block-level structure.

### Block Parser (`blockparser.h / blockparser.cpp`)
Walks the token list and groups tokens into `blockNode` structs representing block-level elements. Each block stores its raw tokens in a `blockToks` vector for later inline parsing. Supported block types:

- `h1`, `h2`, `h3` — headings
- `p` — paragraphs (soft line breaks handled correctly — single `\n` continues the paragraph, blank line ends it)
- `hr` — horizontal rules
- `ul` — unordered lists
- `ol` — ordered lists

### Inline Parser (`inlineparser.h / inlineparser.cpp`)
Runs a second pass over the block tree. For each block that has tokens, it calls `parseInlineChildren(-1)` which recursively descends into delimiter pairs (`*`, `**`, `***`) to produce a nested tree of `inlineNode` structs stored in each block's `children` vector.

The recursion works via a shared `curTok` cursor and a `closer` parameter — each recursive call owns its own local `children` vector and returns when it hits its closing delimiter. Supported inline types:

- `em` — italic (`*text*`)
- `strong` — bold (`**text**`)
- `strongEm` — bold italic (`***text***`)
- `text` — plain text runs
- `ul` / `ol` — list item markers (kept as typed nodes so the renderer can detect item boundaries)

### Node Types (`nodes.h`)
`blockNode` and `inlineNode` are defined in a shared header to avoid circular dependencies between the two parser headers. `blockNode.children` holds `inlineNode`s. `inlineNode.children` holds further `inlineNode`s for recursive nesting.

## Current State

### Working
- Full tokenization of all markdown syntax characters
- Block detection for all supported block types
- Correct multi-line paragraph handling
- Recursive inline parsing with arbitrary nesting depth (e.g. `*italic **bold** italic*` produces the correct three-child em node)
- Inline formatting inside list items (e.g. `**important**` inside an ol item is correctly parsed as a strong node)
- Tree printer for visualising the AST with indentation reflecting nesting depth

### In Progress
- Link parsing — `[display text](url)` detection is being moved from the tokenizer into the inline parser where it belongs. Will produce a `link` node with `value` = URL and `children` = display text node.
- Sub-list nesting for `ul` (indented items) — currently flat

### Not Yet Started
- HTML renderer — walks the final AST and emits an HTML string
- `del`, `mark`, `sub`, `sup`, `code` inline types (tokenized, not yet parsed)
- Blockquotes

## Build

Requires CMake.

```bash
mkdir build && cd build
cmake ..
make
./first_cmake_program
```

Input is read from `inputfile.md` at the hardcoded path in `loadfile.h`. Update that path to point at your own file.

## Token Types

Defined as an enum in `tokenizer.h`. Block-level: `hr h1 h2 h3 blockquote ul ol p`. Inline: `em strong strongEm del sub sup mark code`. Structural: `newLine space digit word text lBracket rBracket lParen rParen period comma quote dQuote fSlash bSlash colon semiColon`.
