" Vim syntax file
" Language:    Yuni Grammar
" Version: 1.0
" Last Change:    2014/07/02
" Maintainer:  Damien GERARD <damien@iwi.me>

" Into your .vimrc :
" autocmd BufRead,BufNewFile,BufEnter *.ygr set filetype=yuni-grammar nowrap cindent smartindent noexpandtab incsearch

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
	syntax clear
elseif exists("b:current_syntax")
	finish
endif


syn case ignore
syn sync lines=120

syntax keyword ygrQualifiers        important notext inline hidden

" Region folding

"

syntax match ygrOperator     "[\|\(\)]"
syntax match ygrRule         "\_^[a-z0-9_-]\+:"
syntax match ygrToken        "tk-[a-z0-9_-]\+"

syntax region ygrComment     start="//"  end="$"   contains=@Spell





" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_ygr_syn_inits")
	if version < 508
	  let did_ygr_syn_inits = 1
	  command -nargs=+ HiLink hi link <args>
	else
	  command -nargs=+ HiLink hi def link <args>
	endif

	HiLink ygrQualifiers        Boolean
	HiLink ygrToken             Label
	HiLink ygrRule              Statement
	HiLink ygrOperator          Operator
	HiLink ygrComment           Comment

	delcommand HiLink
endif


let b:current_syntax = "yuni-grammar"

" vim: ts=4 sw=2
