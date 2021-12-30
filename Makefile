.PHONY: clean All

All:
	@echo "----------Building project:[ CIFE - Release - MSW static ]----------"
	@cd "CIFE" && "$(MAKE)" -f  "CIFE.mk"
clean:
	@echo "----------Cleaning project:[ CIFE - Release - MSW static ]----------"
	@cd "CIFE" && "$(MAKE)" -f  "CIFE.mk" clean
