
#include "algorithms/young_project_product.hh"
#include "algorithms/young_project_tensor.hh"

young_project_product::young_project_product(Kernel& k, exptree& tr)
	: Algorithm(k, tr)
	{
	}

bool young_project_product::can_apply(iterator it)
	{
	if(*it->name=="\\prod") return true;
	return false;
	}

Algorithm::result_t young_project_product::apply(iterator& it)
	{
	exptree rep;
	iterator topsum = rep.set_head(str_node("\\sum"));

	sibling_iterator sib=tr.begin(it);
	bool first=true;
	while(sib!=tr.end(it)) {
		young_project_tensor ypt(kernel, tr, true);
		sibling_iterator nxt=sib;
		 ++nxt;
		 if(ypt.can_apply(sib)) {
			  ypt.modulo_monoterm=true;
			  iterator ii(sib);
			  ypt.apply(ii);
			  if(*ii->name!="\\sum") 
					ii=tr.wrap(ii, str_node("\\sum"));

			  expression_modified=true;
			  if(first) {
					// Add a new \prod node to rep for each term in the projected factor.
					first=false;
					sibling_iterator trm=tr.begin(ii);
					while(trm!=tr.end(ii)) {
						 iterator prod=rep.append_child(topsum, str_node("\\prod"));
						 prod->fl.bracket=str_node::b_round;
						 prod->multiplier=it->multiplier;
						 rep.append_child(prod, iterator(trm));
						 ++trm;
						 }
					}
			  else {
					// Distribute this projected factor over all existing terms in rep,
					sibling_iterator trm=rep.begin(topsum);
					while(trm!=rep.end(topsum)) {
//						 iterator tmp=trm;
						 sibling_iterator nxttrm=trm;
						 ++nxttrm;

						 // Copy this term out of the sum and append projected factor.
						 exptree work(trm);
						 iterator workit=work.begin();
						 iterator put=rep.append_child(work.begin(), str_node());
						 if(tr.number_of_children(ii)==1)
							  rep.replace(put, tr.begin(ii));
						 else {
							  rep.replace(put, ii);
							  
							  // Distribute the product.
							  distribute   dis(work, work.end());
							  if(dis.can_apply(workit)) 
									dis.apply(workit);
							  }
//						 txtout << "GOOD?" << std::endl;
//						 work.print_recursive_treeform(txtout, work.begin());

						 // Canonicalise all new products.
						 canonicalise can(work, work.end());
//						 txtout << *workit->name << " " << *put->name << std::endl;
						 can.apply_recursive(workit, false);
						 if(*work.begin()->multiplier!=0) {
							  // The upcoming move wants to see a sum, even if there is only one term
							  if(*work.begin()->name!="\\sum")
									work.wrap(work.begin(), str_node("\\sum"));
							  
							  // Move back to the original tree.
							  sibling_iterator cpyit=work.begin(work.begin());
							  while(cpyit!=work.end(work.begin())) {
									sibling_iterator cpyitnxt=cpyit;
									++cpyitnxt;
									rep.move_before(trm, cpyit);
									cpyit=cpyitnxt;
									}
							  }
						 rep.erase(trm);
						 
						 trm=nxttrm;
						 }
					collect_terms coll(rep, rep.end());
					if(coll.can_apply(topsum)) 
						 coll.apply(topsum);

					// If collect terms removed the sum because there was only
					// one term (or zero) left, put it back in.
					if(*topsum->name!="\\sum")
						 topsum=tr.wrap(topsum, str_node("\\sum"));
					}
			  }
		 else {
			  if(first) {
					first=false;
					iterator prod=rep.append_child(topsum, str_node("\\prod"));
					prod->fl.bracket=str_node::b_round;
					prod->multiplier=it->multiplier;
					rep.append_child(prod, iterator(sib));
					}
			  else {
					// just multiply all terms with this factor
					sibling_iterator trm=rep.begin(topsum);
					while(trm!=rep.end(topsum)) {
						 iterator put=rep.append_child(trm,str_node());
						 tr.replace(put, iterator(sib));
						 ++trm;
						 }
					}
			  }
//		 rep.print_recursive_treeform(debugout, rep.begin());
		 sib=nxt;
		 }
	
	if(expression_modified) {
		 it=tr.replace(it, rep.begin());
		 expression_modified=true;
		 // FIXME: this canonicalise should really not be necessary
//		 txtout << "WHOOAAH " << *it->name << std::endl;
//		 tr.print_recursive_treeform(txtout, it);
//		 canonicalise can(tr, tr.end());
//		 can.apply_recursive(it, false);
		 cleanup_sums_products(tr, it);
		 return l_applied;
		 }
	else return l_no_action;
	}
