# C Markdown Parser

A markdown parser built from scratch in C++. It reads a markdown source file and produces a two-level AST — a block tree and a nested inline tree — then walks that tree to emit HTML. Inline formatting nests recursively to arbitrary depth, and list items carry their own inline formatting and sub-lists at any level.

## Pipeline

The parser runs as four sequential passes, each consuming the output of the last:

```
Source file → Tokenizer → Block Parser → Inline Parser → Stringifier → HTML
```

Each stage has a single job and hands a cleaner structure to the next: the tokenizer turns raw text into typed tokens, the block parser groups tokens into block-level elements, the inline parser turns each block's tokens into a nested tree of formatting nodes, and the stringifier walks the finished tree and produces HTML.

### Tokenizer (`tokenizer.h` / `tokenizer.cpp`)

Reads the source string one character at a time and emits a flat list of typed `Token` structs. Each token carries a `type` (from the `TokenType` enum), a `value` string, and an `index` — the token's position within its line. The index resets to `0` at every newline, which is how downstream passes detect line-start structure (a heading marker or list marker only counts when it's the first token on its line).

Newline tokens store a real newline character as their value, so line breaks survive into the HTML output — meaningful inside code blocks, where `<pre>` preserves them.

### Block Parser (`blockparser.h` / `blockparser.cpp`)

Walks the token list and groups tokens into `blockNode` structs, one per block-level element. Most blocks stash their raw tokens in a `blockToks` vector for the inline pass to process later. Supported blocks:

- `h1`–`h6` — headings
- `p` — paragraphs, with correct soft-break handling: a single newline continues the paragraph, a blank line ends it
- `hr` — horizontal rules
- `blockquote` — quoted blocks
- `codeblock` — fenced code blocks (content kept verbatim)
- `ul` — unordered lists, including nested sub-lists detected by indentation depth
- `ol` — ordered lists, including nested sub-lists

List parsing is recursive: `parseUl` and `parseOl` track indentation depth and call themselves when they encounter a more-deeply-indented item, so a list nests as far as the source does.

### Inline Parser (`inlineparser.h` / `inlineparser.cpp`)

Runs a second pass over the block tree. For each block with tokens, it calls `parseInlineChildren(-1)`, which recursively descends into delimiter pairs to build a nested tree of `inlineNode` structs stored in the block's `children`.

The recursion works through a shared `curTok` cursor and a `closer` parameter: each call owns its own local `children` vector and returns when it reaches its closing delimiter, so a run like `*italic **bold** italic*` produces a correctly nested `em` node with three children.

List items get their own recursive treatment through `listCheck`. It is handed a list of items and, for each one, parses that item's inline tokens, then looks through the item's children for a `ul`/`ol` sub-list and calls itself on the sub-list's items. Because it calls itself, inline formatting inside list items works at any nesting depth — `**Green**` inside a second-level list item is parsed as a `strong` node just like it would be at the top level.

Supported inline types:

- `em` — italic (`*text*`)
- `strong` — bold (`**text**`)
- `strongEm` — bold italic (`***text***`)
- `code` — inline code (`` `text` ``)
- `link` — `[display](url)`, parsed here in the inline pass
- `text` — plain text runs
- `ul` / `ol` — list-marker nodes, kept as typed nodes so the renderer can find item boundaries

### Stringifier (`stringifier.h` / `stringifier.cpp`)

Walks the final AST and builds the HTML string. Block types map to their tags (`<h1>`, `<p>`, `<pre><code>`, `<blockquote>`, `<ul>`, `<ol>`, and so on). Inline nodes are rendered by `createInlineHTML`, which recurses through the nested formatting tree and wraps each node in its tag.

Lists are rendered recursively to match the parser: for each item it emits the item's own inline content, then — if the item holds a `ul`/`ol` child — descends into it through `ulLoop` / `olLoop`, producing correctly nested `<ul>`/`<ol>` markup inside the `<li>`.

### Node Types (`nodes.h`)

`blockNode` and `inlineNode` live in a shared header to avoid a circular dependency between the two parser headers. `blockNode.children` holds `inlineNode`s; `inlineNode.children` holds further `inlineNode`s for recursive nesting.

## Current State

### Working

- Full tokenization of the markdown syntax set
- Block detection for headings, paragraphs, horizontal rules, blockquotes, code blocks, and ordered/unordered lists
- Correct multi-line paragraph handling (soft break vs. blank-line break)
- Recursive inline parsing to arbitrary depth (e.g. `*italic **bold** italic*`)
- Inline formatting inside list items at any nesting depth
- Nested lists (indented sub-lists) for both `ul` and `ol`
- Links (`[text](url)`) parsed in the inline pass
- Blockquotes and fenced code blocks rendered to HTML
- Newlines preserved into output for accurate `<pre>` rendering
- HTML output for all of the above
- AST printer (`printTree`) that shows the tree with indentation reflecting nesting depth

### Known Limitations / Next Up

- An item that has *both* its own inline formatting *and* a sub-list: storing the parsed inline children currently overwrites the item's existing children, so a sub-list on a formatted item can be lost. Present markup doesn't hit this, but it's the next structural edge to handle.
- List items render with a leading space carried from the marker (`<li> Apples`).
- Backslash escaping (`\*` for a literal asterisk) is tokenized but not yet honored.
- Not yet parsed, though tokenized: `del`, `mark`, `sub`, `sup`.
- Images (`![alt](url)`), reference-style links, and tables.

## Build

Requires CMake.

```bash
mkdir build && cd build
cmake ..
make
./first_cmake_program
```

Input is read from `inputfile.md` at the hardcoded path in `loadfile.h` — update that path to point at your own file.

## Token Types

Defined as an enum in `tokenizer.h`.

- **Block-level:** `hr h1 h2 h3 h4 h5 h6 codeblock blockquote ul ol p`
- **Inline:** `em strong strongEm del sub sup mark code link`
- **Structural:** `newLine space digit word text indent lBracket rBracket lParen rParen period comma quote dQuote fSlash bSlash colon semiColon`
